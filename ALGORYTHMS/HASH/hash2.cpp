/* Отделил C-инклюды и упорядочил инклюды по алфавиту. */
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <memory.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <vector>



using namespace std;
using chrono::duration;
using chrono::system_clock;

/*
 * vs  — [v]ector of [s]trings
 * vvs — [v]ector of [v]ectors of [s]trings
 */
using vs_t = vector<string>;
using vvs_t = vector<vs_t>;

/*
 * Чтобы не путать их,
 * хеш-значения будут иметь тип hash_t,
 * а позиции в реализациях хеш-структур будут иметь тип index_t.
 * На самом деле, это не более чем комментарии,
 * так как типы совместимы и компилятор не будет ругаться если их перепутать.
 */
using hash_t = uint64_t;
using index_t = size_t;



class debug_t
{
public:
    bool enabled;
};

template <typename T>
debug_t& operator<<(debug_t& debug,
                    const T& value)
{
    if (debug.enabled)
        cout << value;
    return debug;
}

debug_t debug;



default_random_engine re;



/*
 * Из объектов произвольной природы (в данном случе из строк)
 * нужно сначала сделать хеш-значения — числа.
 */
hash_t string2hash(const string& string)
{
    /*
     * Хеш-функцию на строках можно задать, например, так.
     * Каждый символ умножим на некоторую константу k >= 2, возведённую в степень, равную позиции этого символа
     * (если считать эти позиции с единицы).
     * И просуммируем эти произведения.
     *
     * Чем хорош такой метод – так как все k^n разные, то хеш-значение изменится,
     * если поменять два символа местами.
     * Но если учесть, что все вычисления мы будет производить не в бесконечном множестве целых чисел,
     * а в типе hash_t, нужно так выбрать k, чтобы все k^n были разными
     * (точнее заметали всё или почти всё множество вычетов по модулю N – {0, ..., N - 1}).
     * В нашем случае, вычисления производятся по модулю N = 1 << 64 == numeric_limits<hash_t>::max() + 1.
     * Так что достаточно взять k, взаимно простое с N.
     *
     * Кроме того, хочется чтобы хеш-значения были равномерно распределены среди всех значений hash_t.
     * Хочется потому что идеальная хеш-функция выдаёт для каждого хешируемого объекта в качестве
     * хеш-значения случайное значение, равномерно распределённое по множеству всех хеш-значений.
     *
     * Если мы возьмём небольшое k, например, равное 3, то при вычислении хеша достаточно коротких строк,
     * будут получаться небольшие хеш-значения, старшие биты которых равны нулю.
     * Это не даст равномерности хеш-значений в hash_t.
     * По той же причине надо возводить k в степени, большие нулевой (поэтому нужно считать позиции с единицы).
     * Если один из символов умножать на k^0 == 1, то строки, различающиеся только в этом символе,
     * будут давать близкие друг к другу хеш-значения.
     * Так как коды самих символов близки по сравнению с полным диапазоном hash_t.
     * И опять не будет похожести на равномерно распределённую случайную величину.
     * Также, если мы возьмём большое k, но делящаяся на (1 << l),
     * то l младших битов хеш-значения будут равны нулю для любой строки.
     * Тут опять поможет взаимная простота k с N.
     * Интуитивно мне кажется, что чем случайнее распределены 0 и 1 в значениях разрядов k,
     * тем равномернее будут распределены хеш-значения.
     * Т.е. лучше всего взять в качестве k число, близкое к N * q,
     * где q в двоичной системе записывается как 0.<тут случайный набор нулей и единиц>.
     * Например, можно брать в качестве q иррациональные числа, не слишком близкие к рациональным.
     * (В том смысле, что случайность нулей и единиц должна начинаться сразу после запятой.
     * А реальная иррациональность – бесконечный хвост – нас не интересует.)
     *
     * Заглянем чуть дальше.
     * Мы будет приводить хеш-значения к индексам в некоторых векторах.
     * При этом вероятно будут производиться вычисления по модулю M – размер вектора.
     * Если k не будет взаимно просто с M, то множество всех хеш-значений по модулю M может
     * стать меньше чем {0, ..., M - 1}.
     * (По крайней мере если хеш-значения считаются не по модулю N, а в бесконечном поле целых чисел.)
     * Эффект тот же, как с делителями нуля, который я описал чуть ниже.
     *
     * В итоге про k имеем:
     * 1) k взаимно просто с N == 1 << 64;
     * 2) k взаимно просто с неизвестными сейчас и разными M;
     * 3) k примерно равно N * q, где q – иррациональное число между 0 и 1, и не слишком близкое к рациональным.
     *
     * 1) + 2) легко удовлетворить взяв в качестве k простое число > 2.
     * Кнут любит золотое сечение – возьмём его в качестве q, оно ничем не хуже любого другого иррационального,
     * в нём есть sqrt(5).
     */

    /*
     * Простое число, близкое к
     * (1 << 64) * (sqrt(5) - 1) / 2.
     * Тут предполагается, что hash_t — 64-х битное.
     */
    const hash_t prime = 0x9E3779B97F4A7BB9; /* == 0b1001111000110111011110011011100101111111010010100111101110111001 */

    hash_t prime_power = 1;
    hash_t result = 0;

    for (auto c : string)
    {
        prime_power *= prime;
        result += prime_power * c;
    }

    return result;
}



/*
 * Этот функтор фактически преобразует одну хеш-функцию в другую хеш-функцию.
 * То есть, имея одну хеш-функцию сложного объекта (например, строки),
 * можно получить целое семейство хеш-функций.
 * Достаточно взять композиции исходной хеш-функции и lfp_t с разными значениями (a, b):
 * lfp_a1_b1(hash_function(object)), lfp_a2_b2(hash_function(object)), ...
 *
 * Реализуется он как линейная функция f(x) = (a * x + b) для a != 0 над кольцом вычетов по простому модулю
 * (т.е. над величинами (x % prime) ).
 * Дело в том, что кольцо вычетов является полем, а значит в нём можно делить на любой элемент, не равный нулю.
 * Вообще говоря, в кольце вычетов делить можно не на все ненулевые элементы. Нельзя делить на делители нуля.
 * Например, в кольце вычетов по модулю 15, элемент 15 – это тот же элемент что и 0.
 * Его делители 3 и 5, оба не равные нулю по модулю 15, являются делителями нуля: 3 * 5 == 0 (mod 15).
 * Поэтому делить на них нельзя (как и на сам ноль) - эта операция не имеет однозначного ответа.
 * 3 * 0 == 3 * 5 == 3 * 10 == 0 (mod 15), 3 * 1 == 3 * 6 == 3 * 11 == 3 (mod 15), и т.д.
 * Следовательно, 0 / 3 по модулю 15 может быть равен как 0, так и 5 или 10.
 * А 3 / 3 по модулю 15 может быть равно как 1, так и 6 или 11.
 * Значит, если рассмотреть функцию g(x) = 3 * x (mod 15),
 * она не будет взаимно однозначно отображать кольцо вычетов по модулю 15 на себя.
 * Всевозможные значения этой функции не будут покрывать все 15 элементов кольца.
 * Этих значений будет всего 5, а не 15.
 * Это будут кратные 3-ке величины: 0 == g(0) == g(5) == g(10), 3 == g(1) == ...
 * Таким образом, если мы возьмём композицию функции g() и функции hash(), принимающую 15 различных значений {0, ..., 14} –
 * количество коллизий хеш-функции hash() возрастёт в 3 раза,
 * т.к. g(hash(...)) будет уже принимать уже только 5 различных значений {0, 3, 6, 9, 12}.
 *
 * А вот в кольце вычетов по простому модулю нет делителей нуля,
 * так как простое число не раскладывается в произведение меньших его натуральных чисел.
 * Поэтому это кольцо является полем, обозначается Fp –
 * F у которой в нижнем индексе стоит или буква p или собственно простое число.
 * И делить там можно на любой элемент, не равный нулю.
 * А значит, f(x) = (a * x + b) (mod prime) является обратимой для любого a != 0: x = (f(x) - b) / a (mod prime).
 * Следовательно, функции этого семейства являются биекциями множества вычетов {0, ..., prime - 1} на себя.
 * А значит, f(hash(...)) пробегает столько же различных значений, сколько и hash(...) .
 * Считая, что результат hash(...) – это вычет по модулю prime.
 * Для чего достаточно взять простое prime, большее чем максимальное значение хеш-функции.
 * Т.е. композиция с f() не увеличивает количество коллизий любой хеш-функции с таким диапазоном значений.
 *
 * Конечно, всё это верно только если prime – действительно простое, а вычисления произведены без переполнений.
 *
 * Это и делает функтор lfp_t замечательным – он не вносит дополнительных коллизий в хеш-функцию,
 * значения которой лежат в диапазоне от 0 до prime - 1.
 * (Единственное что, в моей реализации prime гораздо меньше чем максимальное значение хеш-функции,
 * и это увеличивает число коллизий в огромное количество раз.)
 */
class lfp_t /* lfp – [l]inear function over [Fp] */
{
public:
    using fp_t = uint64_t;

    lfp_t() :
    a(uniform_int_distribution<fp_t>(1, prime - 1)(re)),
    b(uniform_int_distribution<fp_t>(0, prime - 1)(re))
    {
    }

    string name() const
    {
        ostringstream s;
        s << "(a = " << a << ", b = " << b << ")";
        return s.str();
    }

    fp_t operator()(fp_t x) const
    {
        /*
         * Производим вычисления в поле вычетов по простому модулю, чуть большему чем (1 << 31), — fp_t.
         * Точнее делаем вид — на самом деле считаем в uint64_t по модулю prime.
         * Так как в этой формуле 64-битное переполнение невозможно,
         * вычисления дают тот же результат, как в конечном поле.
         * Если считать честно (и тогда можно брать prime до (1 << 63) - 1), то нужно делать что-то вроде
         * http://en.wikipedia.org/wiki/Modular_arithmetic#Example_implementations .
         *
         * А в идеале, нужно считать в длинной арифметике по простому модулю,
         * большему чем numeric_limits<hash_t>::max():
         * http://en.wikipedia.org/wiki/Arbitrary-precision_arithmetic .
         */
        return (a * (x % prime) + b) % prime;
    }

    static const fp_t prime = 0x8000000b;

private:
    const fp_t a;
    const fp_t b;
};



/*
 * Далее, при реализации хеш-структур нам придётся по хеш-значению объекта
 * находить позицию объекта в векторе, используемом реализацией хеш-структуры.
 * Классы-потомки hash2index_t переводят хеш-значение в индекс в векторе заданного размера.
 */
class hash2index_t
{
public:
    virtual string name() const = 0;
    virtual index_t operator()(hash_t hash,
                               index_t size) const = 0;
};

class division_t : public hash2index_t
{
public:
    string name() const override
    {
        /* Пока что по-простому – без typeid, __PRETTY_FUNCTION__, и т.д. */
        return "division_t";
    }

    index_t operator()(hash_t hash,
                       index_t size) const override
    {
        return hash % size;
    }
};

class multiplication_t : public hash2index_t
{
public:
    string name() const override
    {
        return "multiplication_t";
    }

    index_t operator()(hash_t hash,
                       index_t size) const override
    {
        /*
         * Производим вычисления в fixed_point_t.
         * Если нужен детерминированный результат, никаких вычислений в float/double лучше не делать.
         * Считая что он представляет собой число с фиксированной точкой, стоящей ровно посередине.
         * Т.е. старшие 32 разряда считаем целой частью числа, а младшие — дробной.
         */

        if (size > fixed_point_t(1) << integral_bits)
        {
            cerr << "Слишком большой диапазон для позиций в хеше.\n";
            /* Исключениями пока пользоваться не будем. */
            exit(1);
        }

        /* Вычисляем результат в представлении с фиксированной точкой. */
        fixed_point_t result = (hash * golden_ratio % to_fixed_point(1)) * size;

        /* Возвращаем целую часть результата. */
        return result >> fractional_bits;
    }

private:
    using fixed_point_t = uint64_t;

    static fixed_point_t to_fixed_point(index_t x)
    {
        return fixed_point_t(x) << fractional_bits;
    }

    static const unsigned int integral_bits = 32;
    static const unsigned int fractional_bits = 32;

    /*
     * (1 << fractional_bits) * (sqrt(5) - 1) / 2 —
     * представление золотого сечения (предложенного Кнутом) в виде числа с фиксированной точкой.
     */
    static const fixed_point_t golden_ratio = 2654435769;
};

class universal_t : public hash2index_t
{
public:
    string name() const override
    {
        return string("universal_t") + lfp.name();
    }

    universal_t(const lfp_t& lfp) :
    lfp(lfp)
    {
    }

    index_t operator()(hash_t hash,
                       index_t size) const override
    {
        if (size > lfp.prime)
        {
            cerr << "Слишком большой диапазон для позиций в хеше.\n";
            exit(1);
        }

        return lfp(hash) % size;
    }

private:
    const lfp_t& lfp;
};



/*
 * При открытой адресации в хеш-структуре нам потребуется находить позиции,
 * куда можно вставить объект с данным хеш-значением.
 * Классы-потомки iterator_t позволяют итерироваться по таким позициям для заданного хеш-значения.
 */
class iterator_t
{
public:
    string name() const
    {
        return do_name() + lfp.name();
    }

    iterator_t(const lfp_t& lfp) :
    lfp(lfp)
    {
    }

    index_t index(hash_t hash,
                  index_t size,
                  index_t i) const
    {
        return do_index(lfp(hash), size, i);
    }

private:
    virtual string do_name() const = 0;

    virtual index_t do_index(hash_t hash,
                             index_t size,
                             index_t i) const = 0;

    const lfp_t& lfp;
};

class linear_t : public iterator_t
{
public:
    linear_t(const lfp_t& lfp) :
    iterator_t(lfp)
    {
    }

private:
    string do_name() const override
    {
        return "linear_t";
    }

    index_t do_index(hash_t hash,
                     index_t size,
                     index_t i) const override
    {
        return (hash + i) % size;
    }
};

class quadratic_t : public iterator_t
{
public:
    quadratic_t(const lfp_t& lfp) :
    iterator_t(lfp)
    {
    }

private:
    string do_name() const override
    {
        return "quadratic_t";
    }

    index_t do_index(hash_t hash,
                     index_t size,
                     index_t i) const override
    {
        /* Обойдёмся без вычислений с плавающей точкой. */
        return (hash + (i + i * i) / 2) % size;
    }
};

class twin_t : public iterator_t
{
public:
    twin_t(const lfp_t& lfp) :
    iterator_t(lfp)
    {
    }

private:
    string do_name() const override
    {
        return "twin_t";
    }

    index_t do_index(hash_t hash,
                     index_t size,
                     index_t i) const override
    {
        /*
         * Обычно в подобных случаях используются действительно две разные хеш-функции.
         * Например, две lfp_t с разными значениями (a, b).
         * Или, можно взять одну lfp и положить c1 = lfp(hash), c2 = lfp(lfp(hash)).
         * Но, пусть будет как в оригинале.
         */
        index_t c1 = hash % size;
        index_t c2 = hash % (size - 1);
        return (c1 + c2 * i) % size;
    }
};



/* Интерфейс структуры, реализующей множество объектов. */
class set_t
{
public:
    /*
     * Интерфейс можно задать виртуальными фукнциями, которые фиксируют сигнатуру вызова.
     * Реальный код будет написан только в классах-потомках.
     */
    virtual bool search(const string& value) const = 0;
    virtual string name() const = 0;

    /*
     * Другой вариант интерфейсных методов:
     * невиртуальный метод, который делает общую для всех потомков работу,
     * пользуясь перегруженными приватными невиртуальными методами.
     * В результате часть функциональности не может быть изменена в классе-потомке,
     * если вызов идёт через указатель на предка.
     * В данном случае каждый потомок будет печатать строчку "Реализация ".
     * С другой стороны, эту функциональность не нужно дублировать.
     * Впрочем, для этой цели существуют и другие способы.
     * (Но смешивать в одном классе эти два варианта не стоит.)
     */
    void dump(bool dump_empty_slots = true) const
    {
        debug << "Реализация " << name() << " :\n";
        do_dump(dump_empty_slots);
    }

private:
    virtual void do_dump(bool dump_empty_slots) const = 0;
};

class simple_set_t : public set_t
{
public:
    simple_set_t(const vs_t& vs) :
    vs(vs)
    {
    }

    bool search(const string& value) const override
    {
        for (const auto& s : vs)
            if (value == s)
                return true;
        return false;
    }

    string name() const override
    {
        return "simple_set_t";
    }

private:
    void do_dump(bool /*dump_empty_slots*/) const override
    {
        for (const auto& s : vs)
            debug << s << " ";
        debug << "\n";
    }

    vs_t vs;
};

class open_hash_t : public set_t
{
public:
    open_hash_t(const iterator_t& iterator,
                const vs_t& vs) :
    /*
     * Обычно функторы передаются как шаблонные аргументы.
     * Но пока обойдёмся без шаблонов.
     */
    iterator(iterator),
    /*
     * При рехешировании нужно увеличить размер вектра.
     * При инициализации тоже возьмём размер чуть больше требуемого,
     * чтобы был запас на коллизии при вставке элементов.
     * Точнее, чтобы не искать долго куда же вставить элемент если его место уже занято.
     */
    vs(increased_size(vs.size()), empty_slot),
    is_reserved_value_in_set(false)
    {
        for (const auto& s : vs)
            insert(s);
    }

    bool search(const string& value) const override
    {
        /*
         * В учебном коде, можно было и не заморачиваться со вставкой в структуру значений,
         * совпадающий с зарезервированными.
         * Но пусть будет.
         */
        if (value == empty_slot)
            return is_reserved_value_in_set;

        index_t index = find_place_for(value);
        return index < vs.size() and vs[index] == value;
    }

    string name() const override
    {
        return string("open_hash_t") + " / " + iterator.name();
    }

private:
    static index_t increased_size(index_t old_size)
    {
        return old_size * 9 / 8;
    }

    index_t find_place_for(const string& value) const
    {
        if (value == empty_slot)
        {
            cerr << "Ошибка реализации класса: бессмысленно вызывать find_place_for(empty_slot). Есть ли во множестве значение, совпадающее с empty_slot, определяет переменная is_reserved_value_in_set.\n";
            exit(1);
        }

        hash_t hash = string2hash(value);

        for (index_t i = 0; i < vs.size(); ++i)
        {
          index_t index = iterator.index(hash, vs.size(), i);

          if (vs[index] == empty_slot or vs[index] == value)
              return index;
        }

        return vs.size();
    }

    void insert(const string& value)
    {
        if (value == empty_slot)
        {
            is_reserved_value_in_set = true;
            return;
        }

        index_t index = find_place_for(value);

        if (index < vs.size())
            vs[index] = value;
        else
        {
            debug << "Не удалось вставить стоку \"" << value << "\" в " << name()
                << ", попробуем рехешировать.\n";

            open_hash_t rehashed_set(iterator, vs);
            rehashed_set.insert(value);
            swap(rehashed_set.vs, vs);
        }
    }

    void do_dump(bool dump_empty_slots) const override
    {
        for (index_t i = 0; i < vs.size(); ++i)
            if (dump_empty_slots or vs[i] != empty_slot)
                debug << i << " : " << vs[i] << "\n";
    }

    static const string empty_slot;

    const iterator_t& iterator;
    vs_t vs;
    bool is_reserved_value_in_set;
};

const string open_hash_t::empty_slot = "";

class chain_hash_t : public set_t
{
public:
    chain_hash_t(const hash2index_t& hash2index,
                 const vs_t& vs) :
    hash2index(hash2index),
    vvs(vs.size())
    {
        for (const auto& s : vs)
            insert(s);
    }

    bool search(const string& value) const override
    {
        index_t index = hash2index(string2hash(value), vvs.size());
        const vs_t vs = vvs[index];

        for (const auto& s : vs)
            if (s == value)
                return true;

        return false;
    }

    string name() const override
    {
        return string("chain_hash_t") + " / " + hash2index.name();
    }

private:
    void insert(const string& value)
    {
        index_t index = hash2index(string2hash(value), vvs.size());
        vvs[index].push_back(value);
    }

    void do_dump(bool dump_empty_slots) const override
    {
        for (index_t i = 0; i < vvs.size(); ++i)
            if (dump_empty_slots or not vvs[i].empty())
            {
                debug << i << "[" << vvs[i].size() << "] : ";
                for (const auto& s : vvs[i])
                    debug << s << " ";
                debug << "\n";
            }
    }

    const hash2index_t& hash2index;
    vvs_t vvs;
};



using profile_t = map<string, duration<double>>;

bool mesure_search(profile_t& profile,
                   const set_t& set,
                   const string& value)
{
    debug << "Поиск строки \"" << value << "\" в " << set.name() << " : ";

    auto start_time = system_clock::now();
    bool result = set.search(value);
    auto end_time = system_clock::now();

    debug << (result ? "" : "НЕ ") << "найдена\n";

    profile[set.name()] += end_time - start_time;

    return result;
}

void print_timing_result(const profile_t& profile)
{
    multimap<duration<double>, string> sorted_profile;
    for (const auto& i : profile)
        sorted_profile.insert(make_pair(i.second, i.first));

    for (const auto& i : sorted_profile)
        cout << i.first.count() << "s : " << i.second << "\n";
}



/*
 * Можно и так:
 * const string letters("abcdefghijklmnopqrstuvwxyz");
 * ...
 * Но я предпочитаю обойтись без этой таблицы.
 * Тем более, зачем набирать алфавит вручную?
 */
string random_string(size_t max_size)
{
    uniform_int_distribution<size_t> random_size(1, max_size);
    uniform_int_distribution<char> random_letter('a', 'z');

    string s(random_size(re), '?');

    for (auto& c : s)
        c = random_letter(re);

    return s;
}

int main(int argument_count,
         char* argument_vector[])
{
    size_t string_count;
    size_t max_string_size;

    random_device rd;
    unsigned int random_seed = rd();

    /*
     * Есть нормальные библиотеки разбора командной строки.
     * Но пока можно и так.
     */
    if (argument_count < 3 or argument_count > 5)
    {
        cerr << "Запуск: " << argument_vector[0]
            << " [<количество строк> <максимальный размер строк> [<затравка для случайных чисел> [<искомая строка>]]]\n"
            << "  Если указана искомая строка или количество строк не превосходит 64 будет включён отладочный режим.\n";
        return 1;
    }

    {
        istringstream in(argument_vector[1]);
        in >> string_count;
    }
    {
        istringstream in(argument_vector[2]);
        in >> max_string_size;
    }

    if (argument_count >= 4)
    {
        istringstream in(argument_vector[3]);
        in >> random_seed;
    }

    debug.enabled = string_count <= 64 or argument_count >= 5;



    re.seed(random_seed);

    cout << "Параметры: количество строк = " << string_count
        << "; максимальный размер строк = " << max_string_size
        << "; затравка для случайных чисел = " << random_seed << ".\n";

    vs_t vs(string_count);

    debug << "Случайный набор строк: ";
    for (auto& s : vs)
    {
        s = random_string(max_string_size);
        debug << s << " ";
    }
    debug << "\n";



    profile_t profile;
    lfp_t lfp;

    division_t division;
    multiplication_t multiplication;
    universal_t universal(lfp);

    linear_t linear(lfp);
    quadratic_t quadratic(lfp);
    twin_t twin(lfp);

    simple_set_t simple_set(vs);

    /* Не засекаем время, просто вычисляем тип для start_time и end_time. */
    auto start_time = system_clock::now();
    auto end_time = start_time;

    /* Время инициализации разных хешей тоже стоит измерить. */
#define HASH_SET(type, hash, first_argument) \
    start_time = system_clock::now(); \
    type hash(first_argument, vs); \
    end_time = system_clock::now(); \
    profile[string("Initialization of ") + hash.name()] += end_time - start_time;

    HASH_SET(chain_hash_t, chain_hash_division, division);
    HASH_SET(chain_hash_t, chain_hash_multiplication, multiplication);
    HASH_SET(chain_hash_t, chain_hash_universal, universal);
    HASH_SET(open_hash_t, open_hash_linear, linear);
    HASH_SET(open_hash_t, open_hash_quadratic, quadratic);
    HASH_SET(open_hash_t, open_hash_twin, twin);

    const set_t* sets[] = {
        &simple_set,
        &chain_hash_division,
        &chain_hash_multiplication,
        &chain_hash_universal,
        &open_hash_linear,
        &open_hash_quadratic,
        &open_hash_twin
    };



    size_t rounds = debug.enabled ? 1 : 1000;
    size_t hits = 0;

    for (size_t i = 0; i < rounds; ++i)
    {
        string search_string = random_string(max_string_size);

        if (argument_count >= 5)
            search_string = argument_vector[4];
        else if (debug.enabled)
        {
            cout << "Введите искомую строку: ";
            cin >> search_string;
        }

        bool correct_search_result = simple_set.search(search_string);
        if (correct_search_result)
            ++hits;

        for (const auto& set : sets)
        {
            set->dump();
            bool search_result = mesure_search(profile, *set, search_string);

            if (search_result != correct_search_result)
            {
                cerr << "Ошибка!\n"
                    << "Поиск строки \"" << search_string << "\" в \n" << set->name() << "\n"
                    << "вернул неправильный результат.\n"
                    << "Для репродуцирования ошибки можно перезапустить так:\n"
                    << argument_vector[0] << " " << string_count << " " << max_string_size << " " << random_seed << " \"" << search_string << "\"\n";
                exit(1);
            }
        }
    }



    cout << "Выполнено " << rounds << " итераций поиска, в "
        << hits << " из них искомые строки найдены.\n"
        << "Результаты профилирования:\n";
    print_timing_result(profile);



    return 0;
}
