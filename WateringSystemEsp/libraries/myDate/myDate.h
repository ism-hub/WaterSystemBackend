/*
 * myDate.h
 *
 *  Created on: 12 срхїз 2017
 *      Author: IsM
 */

#ifndef LIBRARIES_MYDATE_MYDATE_H_
#define LIBRARIES_MYDATE_MYDATE_H_

#include <chrono>

#  define CONSTDATA constexpr const
#  define CONSTCD11 constexpr
#  define CONSTCD14
#  define MYNEXCEPT11 noexcept

namespace myDate {




//###### The inner clock

struct my_clock
{
  typedef std::chrono::duration<uint32_t>				duration;//seconds with uint32_t as data type
  typedef duration::rep	  								rep;
  typedef duration::period	  							period;
  typedef std::chrono::time_point<my_clock, duration> 	time_point;

  static constexpr bool is_steady = false;


  static void setCurrentUnixTime(time_point unixTimeNow){
	  _unixTimestampOnSystemStart = unixTimeNow - std::chrono::duration_cast<duration>(std::chrono::milliseconds{millis()});//TODO: the millis is ulong and it can operate only for 50 days before the restart
  }

  static time_point now() noexcept{
	  return _unixTimestampOnSystemStart + std::chrono::duration_cast<duration>(std::chrono::milliseconds(millis()));
  }

	// Map to C API
	static std::time_t to_time_t(const time_point& __t) noexcept
	{

		return std::time_t(std::chrono::duration_cast<std::chrono::duration<long> > (__t.time_since_epoch()).count());//time_t is long type
	}

	static time_point from_time_t(std::time_t __t) noexcept
	{
		typedef std::chrono::time_point<my_clock, std::chrono::duration<long>> __from;
		return std::chrono::time_point_cast<duration> (__from(std::chrono::duration<long>(__t)));
	}

private:
  static time_point _unixTimestampOnSystemStart;
};

my_clock::time_point my_clock::_unixTimestampOnSystemStart = my_clock::time_point{my_clock::duration{0}};

//##### end our clock



using days = std::chrono::duration
    <int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

using weeks = std::chrono::duration
    <int, std::ratio_multiply<std::ratio<7>, days::period>>;

using years = std::chrono::duration
    <int, std::ratio_multiply<std::ratio<146097, 400>, days::period>>;

using months = std::chrono::duration
    <int, std::ratio_divide<years::period, std::ratio<12>>>;

template <class Duration>
    using sys_time = std::chrono::time_point<my_clock, Duration>;

using sys_days    = sys_time<days>;

struct local_t {};

template <class Duration>
    using local_time = std::chrono::time_point<local_t, Duration>;

using local_seconds = local_time<std::chrono::seconds>;
using local_days    = local_time<days>;


//####### time_of_day stuff

template <class Duration>
class time_of_day_storage;

template <class Rep, class Period>
class time_of_day_storage<std::chrono::duration<Rep, Period>>
{

    std::chrono::hours   h_;
    std::chrono::minutes m_;
    std::chrono::seconds s_;


public:
    using precision = std::chrono::seconds;

    constexpr time_of_day_storage() noexcept
        : h_(0)
        , m_(0)
        , s_(0)
        {}

    constexpr explicit time_of_day_storage(std::chrono::seconds since_midnight) noexcept
        : h_(std::chrono::duration_cast<std::chrono::hours>(since_midnight))
        , m_(std::chrono::duration_cast<std::chrono::minutes>(since_midnight - h_))
        , s_(since_midnight - h_ - m_)
        {}

    constexpr explicit time_of_day_storage(std::chrono::hours h, std::chrono::minutes m, std::chrono::seconds s) noexcept
        : h_(h)
        , m_(m)
        , s_(s)
        {}

    constexpr std::chrono::hours hours() const noexcept {return h_;}
    constexpr std::chrono::minutes minutes() const noexcept {return m_;}
    constexpr std::chrono::seconds seconds() const noexcept {return s_;}
};


template <class Duration>
class time_of_day: public time_of_day_storage<Duration>
{
    using base = time_of_day_storage<Duration>;
public:
    constexpr time_of_day() noexcept = default;

    constexpr explicit time_of_day(Duration since_midnight) noexcept
        : base(since_midnight)
        {}

    template <class Arg0, class Arg1, class ...Args>
	constexpr explicit time_of_day(Arg0&& arg0, Arg1&& arg1, Args&& ...args) noexcept
        : base(std::forward<Arg0>(arg0), std::forward<Arg1>(arg1), std::forward<Args>(args)...)
        {}
};

template <class Rep, class Period, class = typename std::enable_if <!std::chrono::treat_as_floating_point<Rep>::value>::type>
constexpr inline time_of_day<std::chrono::duration<Rep, Period>> make_time(const std::chrono::duration<Rep, Period>& d)
{
    return time_of_day<std::chrono::duration<Rep, Period>>(d);
}
/*
//######### year_month_day stuff
class year
{
    short y_;

public:
    year() = default;
   // explicit constexpr year(int y) noexcept;

    constexpr inline year(int y) noexcept : y_(static_cast<decltype(y_)>(y)) {}
     inline myDate::year& operator++() noexcept {++y_; return *this;}
     inline myDate::year operator++(int) noexcept {auto tmp(*this); ++(*this); return tmp;}
     inline myDate::year& operator--() noexcept {--y_; return *this;}
     inline myDate::year operator--(int) noexcept {auto tmp(*this); --(*this); return tmp;}
     inline myDate::year& operator+=(const years& y) noexcept ;

     inline myDate::year& operator-=(const years& y) noexcept;// {*this = *this - y; return *this;}
    constexpr inline myDate::year operator-() const noexcept {return myDate::year{-y_};}
    constexpr inline myDate::year operator+() const noexcept {return *this;}

     constexpr bool is_leap() const noexcept
	{
		return y_ % 4 == 0 && (y_ % 100 != 0 || y_ % 400 == 0);
	}

     constexpr explicit operator int() const noexcept{return y_;}
     constexpr bool ok() const noexcept
	{
		return y_ != std::numeric_limits<short>::min();
	}

    static constexpr myDate::year min() noexcept {return myDate::year{-32767};}
    static constexpr myDate::year max() noexcept {return myDate::year{32767};}

};

constexpr inline years operator-(const year& x, const year& y) noexcept
    {
        return years{static_cast<int>(x) - static_cast<int>(y)};
    }

    constexpr inline year operator+(const year& x, const years& y) noexcept
    {
        return year{static_cast<int>(x) + y.count()};
    }

    constexpr inline year operator+(const years& x, const year& y) noexcept
    {
        return y + x;
    }

    constexpr inline year
    operator-(const year& x, const years& y) noexcept
    {
        return year{static_cast<int>(x) - y.count()};
    }

constexpr inline bool operator==(const year& x, const year& y) noexcept
    {
        return static_cast<int>(x) == static_cast<int>(y);
    }

    constexpr inline bool operator!=(const year& x, const year& y) noexcept
    {
        return !(x == y);
    }

    constexpr inline bool operator<(const year& x, const year& y) noexcept
    {
        return static_cast<int>(x) < static_cast<int>(y);
    }

    constexpr inline bool operator>(const year& x, const year& y) noexcept
    {
        return y < x;
    }

    constexpr inline bool operator<=(const year& x, const year& y) noexcept
    {
        return !(y < x);
    }

    constexpr inline bool operator>=(const year& x, const year& y) noexcept
    {
        return !(x < y);
    }
     inline year& year::operator+=(const years& y) noexcept {*this = *this + y; return *this;}
      inline year& year::operator-=(const years& y) noexcept {*this = *this - y; return *this;}


class month
{
    unsigned char m_;

public:
    month() = default;
    explicit constexpr month(unsigned m) noexcept;

     inline month& operator++() noexcept {if (++m_ == 13) m_ = 1; return *this;}
     inline month operator++(int) noexcept {auto tmp(*this); ++(*this); return tmp;}
     inline month& operator--() noexcept {if (--m_ == 0) m_ = 12; return *this;}
     inline month operator--(int) noexcept {auto tmp(*this); --(*this); return tmp;}

     constexpr explicit operator unsigned() const noexcept;
     constexpr inline bool ok() const noexcept;

     inline month& operator+=(const months& m) noexcept;

     inline month& operator-=(const months& m) noexcept;

};
constexpr inline month::month(unsigned m) noexcept : m_(static_cast<decltype(m_)>(m)) {}

    constexpr inline bool operator==(const month& x, const month& y) noexcept
    {
        return static_cast<unsigned>(x) == static_cast<unsigned>(y);
    }

    constexpr inline bool operator!=(const month& x, const month& y) noexcept
    {
        return !(x == y);
    }

    constexpr inline bool operator<(const month& x, const month& y) noexcept
    {
        return static_cast<unsigned>(x) < static_cast<unsigned>(y);
    }

    constexpr inline bool operator>(const month& x, const month& y) noexcept
    {
        return y < x;
    }

    constexpr inline bool operator<=(const month& x, const month& y) noexcept
    {
        return !(y < x);
    }

    constexpr inline bool operator>=(const month& x, const month& y) noexcept
    {
        return !(x < y);
    }

    inline months operator-(const month& x, const month& y) noexcept
    {
        auto const d = static_cast<unsigned>(x) - static_cast<unsigned>(y);
        return months(d <= 11 ? d : d + 12);
    }

    inline month operator+(const month& x, const months& y) noexcept
    {
        auto const mu = static_cast<long long>(static_cast<unsigned>(x)) - 1 + y.count();
        auto const yr = (mu >= 0 ? mu : mu-11) / 12;
        return month{static_cast<unsigned>(mu - yr * 12 + 1)};
    }

    inline month operator+(const months& x, const month& y) noexcept
    {
        return y + x;
    }

    inline month operator-(const month& x, const months& y) noexcept
    {
        return x + -y;
    }

    constexpr inline month::operator unsigned() const noexcept {return m_;}
    constexpr inline bool month::ok() const noexcept {return 1 <= m_ && m_ <= 12;}

    inline month& month::operator+=(const months& m) noexcept
    {
	   *this = *this + m;
	   return *this;
     }

    inline month& month::operator-=(const months& m) noexcept
    {
	   *this = *this - m;
	   return *this;
    }


class day
{
    unsigned char d_;

public:
    day() = default;
    explicit constexpr inline day(unsigned d) noexcept : d_(static_cast<unsigned char>(d)) {}
     inline day& operator++() noexcept {++d_; return *this;}
     inline day operator++(int) noexcept {auto tmp(*this); ++(*this); return tmp;}
     inline day& operator--() noexcept {--d_; return *this;}
     inline day operator--(int) noexcept {auto tmp(*this); --(*this); return tmp;}
     inline day& operator+=(const days& d) noexcept;// {*this = *this + d; return *this;}
     inline day& operator-=(const days& d) noexcept;// {*this = *this - d; return *this;}
     constexpr inline operator unsigned() const noexcept;// {return d_;}
     constexpr inline bool ok() const noexcept;// {return 1 <= d_ && d_ <= 31;}

};

constexpr inline day::operator unsigned() const noexcept {return d_;}
constexpr inline bool day::ok() const noexcept {return 1 <= d_ && d_ <= 31;}

constexpr inline bool operator==(const day& x, const day& y) noexcept
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

constexpr inline bool operator!=(const day& x, const day& y) noexcept
{
    return !(x == y);
}

constexpr inline bool operator<(const day& x, const day& y) noexcept
{
    return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

constexpr inline bool operator>(const day& x, const day& y) noexcept
{
    return y < x;
}

constexpr inline bool operator<=(const day& x, const day& y) noexcept
{
    return !(y < x);
}

constexpr inline bool operator>=(const day& x, const day& y) noexcept
{
    return !(x < y);
}

constexpr inline days operator-(const day& x, const day& y) noexcept
{
    return days{static_cast<days::rep>(static_cast<unsigned>(x) - static_cast<unsigned>(y))};
}

constexpr inline day operator+(const day& x, const days& y) noexcept
{
    return day{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

constexpr inline day operator+(const days& x, const day& y) noexcept
{
    return y + x;
}

constexpr inline day operator-(const day& x, const days& y) noexcept
{
    return x + -y;
}

inline day& day::operator+=(const days& d) noexcept {*this = *this + d; return *this;}
inline day& day::operator-=(const days& d) noexcept {*this = *this - d; return *this;}


struct last_spec
{
    explicit last_spec() = default;
};

constexpr const myDate::last_spec last{};

constexpr const month jan{1};
constexpr const month feb{2};
constexpr const month mar{3};
constexpr const month apr{4};
constexpr const month may{5};
constexpr const month jun{6};
constexpr const month jul{7};
constexpr const month aug{8};
constexpr const month sep{9};
constexpr const month oct{10};
constexpr const month nov{11};
constexpr const month dec{12};

class year_month
{
	myDate::year  y_;
	myDate::month m_;

public:
    year_month() = default;
    constexpr year_month(const myDate::year& y, const myDate::month& m) noexcept;

    constexpr myDate::year  year()  const noexcept;
    constexpr myDate::month month() const noexcept;

     myDate::year_month& operator+=(const months& dm) noexcept;
     myDate::year_month& operator-=(const months& dm) noexcept;
     myDate::year_month& operator+=(const years& dy) noexcept;
     myDate::year_month& operator-=(const years& dy) noexcept;

    constexpr bool ok() const noexcept;
};

constexpr bool operator==(const year_month& x, const year_month& y) noexcept;
constexpr bool operator!=(const year_month& x, const year_month& y) noexcept;
constexpr bool operator< (const year_month& x, const year_month& y) noexcept;
constexpr bool operator> (const year_month& x, const year_month& y) noexcept;
constexpr bool operator<=(const year_month& x, const year_month& y) noexcept;
constexpr bool operator>=(const year_month& x, const year_month& y) noexcept;

 myDate::year_month operator+(const year_month& ym, const months& dm) noexcept;
 year_month operator+(const months& dm, const year_month& ym) noexcept;
 year_month operator-(const year_month& ym, const months& dm) noexcept;

constexpr myDate::months operator-(const year_month& x, const year_month& y) noexcept;
constexpr myDate::year_month operator+(const year_month& ym, const years& dy) noexcept;
constexpr myDate::year_month operator+(const years& dy, const year_month& ym) noexcept;
constexpr myDate::year_month operator-(const year_month& ym, const years& dy) noexcept;



// year_month impl

CONSTCD11
inline
year_month::year_month(const myDate::year& y, const myDate::month& m) MYNEXCEPT11
    : y_(y)
    , m_(m)
    {}

CONSTCD11 inline year year_month::year() const MYNEXCEPT11 {return y_;}
CONSTCD11 inline month year_month::month() const MYNEXCEPT11 {return m_;}
CONSTCD11 inline bool year_month::ok() const MYNEXCEPT11 {return y_.ok() && m_.ok();}


inline myDate::year_month& year_month::operator+=(const months& dm) MYNEXCEPT11
{
    *this = *this + dm;
    return *this;
}

CONSTCD14
inline
year_month&
year_month::operator-=(const months& dm) MYNEXCEPT11
{
    *this = *this - dm;
    return *this;
}

CONSTCD14
inline
year_month&
year_month::operator+=(const years& dy) MYNEXCEPT11
{
    *this = *this + dy;
    return *this;
}

CONSTCD14
inline
myDate::year_month&
year_month::operator-=(const years& dy) MYNEXCEPT11
{
    *this = *this - dy;
    return *this;
}

CONSTCD11
inline
bool
operator==(const year_month& x, const year_month& y) MYNEXCEPT11
{
    return x.year() == y.year() && x.month() == y.month();
}

CONSTCD11
inline
bool
operator!=(const year_month& x, const year_month& y) MYNEXCEPT11
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const year_month& x, const year_month& y) MYNEXCEPT11
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.month() < y.month()));
}

CONSTCD11
inline
bool
operator>(const year_month& x, const year_month& y) MYNEXCEPT11
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const year_month& x, const year_month& y) MYNEXCEPT11
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const year_month& x, const year_month& y) MYNEXCEPT11
{
    return !(x < y);
}

CONSTCD14
inline
year_month
operator+(const year_month& ym, const months& dm) MYNEXCEPT11
{
    auto dmi = static_cast<int>(static_cast<unsigned>(ym.month())) - 1 + dm.count();
    auto dy = (dmi >= 0 ? dmi : dmi-11) / 12;
    dmi = dmi - dy * 12 + 1;
    return year_month(year(ym.year() + years(dy)), month(static_cast<unsigned>(dmi)));
}

CONSTCD14
inline
year_month
operator+(const months& dm, const year_month& ym) MYNEXCEPT11
{
    return ym + dm;
}

CONSTCD14
inline
year_month
operator-(const year_month& ym, const months& dm) MYNEXCEPT11
{
    return ym + -dm;
}

CONSTCD11
inline
months
operator-(const year_month& x, const year_month& y) MYNEXCEPT11
{
    return (x.year() - y.year()) +
            months(static_cast<unsigned>(x.month()) - static_cast<unsigned>(y.month()));
}

CONSTCD11
inline
year_month
operator+(const year_month& ym, const years& dy) MYNEXCEPT11
{
    return year_month((ym.year() + dy) , ym.month());
}

CONSTCD11
inline
year_month
operator+(const years& dy, const year_month& ym) MYNEXCEPT11
{
    return ym + dy;
}

CONSTCD11
inline
year_month
operator-(const year_month& ym, const years& dy) MYNEXCEPT11
{
    return ym + -dy;
}



// month_day_last

class month_day_last
{
    myDate::month m_;

public:
    constexpr inline month_day_last(const myDate::month& m) noexcept : m_(m) {}
    constexpr inline bool ok() const noexcept {return m_.ok();}
    constexpr inline myDate::month month() const noexcept {return m_;}

};

constexpr inline bool operator==(const month_day_last& x, const month_day_last& y) noexcept
{
    return x.month() == y.month();
}

constexpr inline bool operator!=(const month_day_last& x, const month_day_last& y) noexcept
{
    return !(x == y);
}

constexpr inline bool operator<(const month_day_last& x, const month_day_last& y) noexcept
{
    return x.month() < y.month();
}

constexpr inline bool operator>(const month_day_last& x, const month_day_last& y) noexcept
{
    return y < x;
}

constexpr inline bool operator<=(const month_day_last& x, const month_day_last& y) noexcept
{
    return !(y < x);
}

constexpr inline bool operator>=(const month_day_last& x, const month_day_last& y) noexcept
{
    return !(x < y);
}

// year_month_day_last


class year_month_day_last
{
    myDate::year           y_;
    myDate::month_day_last mdl_;

public:
    constexpr inline year_month_day_last(const myDate::year& y, const myDate::month_day_last& mdl) noexcept;


    inline myDate::year_month_day_last& operator+=(const months& m) noexcept;


    inline myDate::year_month_day_last& operator-=(const months& m) noexcept;


    inline myDate::year_month_day_last& operator+=(const years& y) noexcept;


    inline myDate::year_month_day_last& operator-=(const years& y) noexcept;


    constexpr inline myDate::year year() const noexcept;// {return y_;}
    constexpr inline myDate::month month() const noexcept;// {return mdl_.month();}
    constexpr inline myDate::month_day_last month_day_last() const noexcept;//{return mdl_;}

    inline myDate::day day() const noexcept;


    inline operator sys_days() const noexcept;


    inline operator local_days() const noexcept;


    constexpr inline bool ok() const noexcept;

};


// year_month_day

class year_month_day
{
	myDate::year  y_;
	myDate::month m_;
	myDate::day   d_;

public:
    year_month_day() = default;
    constexpr inline year_month_day(const myDate::year& y, const myDate::month& m, const myDate::day& d) noexcept
        : y_(y)
        , m_(m)
        , d_(d)
        {}

    inline year_month_day(const year_month_day_last& ymdl) noexcept;

     inline year_month_day(sys_days dp) noexcept
        : year_month_day(from_days(dp.time_since_epoch()))
        {}

  inline year_month_day(local_days dp) noexcept
        : year_month_day(from_days(dp.time_since_epoch()))
        {}

  inline year_month_day& operator+=(const months& m) noexcept;


  inline year_month_day& operator-=(const months& m) noexcept;


  inline year_month_day& operator+=(const years& y) noexcept;


  inline year_month_day& operator-=(const years& y) noexcept;


     constexpr inline myDate::year year() const noexcept {return y_;}
     constexpr inline myDate::month month() const noexcept {return m_;}
     constexpr inline myDate::day day() const noexcept {return d_;}

     inline operator sys_days() const noexcept
     {
         return sys_days{to_days()};
     }

     inline operator local_days() const noexcept
     {
         return local_days{to_days()};
     }

     inline bool ok() const noexcept;



private:

    static inline year_month_day from_days(days dp) noexcept
    {
        static_assert(std::numeric_limits<unsigned>::digits >= 18,
                 "This algorithm has not been ported to a 16 bit unsigned integer");
        static_assert(std::numeric_limits<int>::digits >= 20,
                 "This algorithm has not been ported to a 16 bit signed integer");
        auto const z = dp.count() + 719468;
        auto const era = (z >= 0 ? z : z - 146096) / 146097;
        auto const doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096]
        auto const yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
        auto const y = static_cast<days::rep>(yoe) + era * 400;
        auto const doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
        auto const mp = (5*doy + 2)/153;                                   // [0, 11]
        auto const d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
        auto const m = mp < 10 ? mp+3 : mp-9;                              // [1, 12]
        return year_month_day{myDate::year{y + (m <= 2)}, myDate::month(m), myDate::day(d)};
    }

    inline days to_days() const noexcept
        {
            static_assert(std::numeric_limits<unsigned>::digits >= 18,
                     "This algorithm has not been ported to a 16 bit unsigned integer");
            static_assert(std::numeric_limits<int>::digits >= 20,
                     "This algorithm has not been ported to a 16 bit signed integer");
            auto const y = static_cast<int>(y_) - (m_ <= feb);
            auto const m = static_cast<unsigned>(m_);
            auto const d = static_cast<unsigned>(d_);
            auto const era = (y >= 0 ? y : y-399) / 400;
            auto const yoe = static_cast<unsigned>(y - era * 400);       // [0, 399]
            auto const doy = (153*(m > 2 ? m-3 : m+9) + 2)/5 + d-1;      // [0, 365]
            auto const doe = yoe * 365 + yoe/4 - yoe/100 + doy;          // [0, 146096]
            return days{era * 146097 + static_cast<int>(doe) - 719468};
        }

};











// year_month_day impl

constexpr inline bool operator==(const year_month_day& x, const year_month_day& y) noexcept
{
    return x.year() == y.year() && x.month() == y.month() && x.day() == y.day();
}

constexpr inline bool operator!=(const year_month_day& x, const year_month_day& y) noexcept
{
    return !(x == y);
}

constexpr inline bool operator<(const year_month_day& x, const year_month_day& y) noexcept
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.month() < y.month() ? true
        : (x.month() > y.month() ? false
        : (x.day() < y.day()))));
}

constexpr inline bool operator>(const year_month_day& x, const year_month_day& y) noexcept
{
    return y < x;
}

constexpr inline bool operator<=(const year_month_day& x, const year_month_day& y) noexcept
{
    return !(y < x);
}

constexpr inline bool operator>=(const year_month_day& x, const year_month_day& y) noexcept
{
    return !(x < y);
}

inline year_month_day operator+(const year_month_day& ymd, const months& dm) noexcept
{
    return year_month_day(ymd.to_days() + dm);
}

inline year_month_day operator+(const months& dm, const year_month_day& ymd) noexcept
{
    return ymd + dm;
}

inline year_month_day operator-(const year_month_day& ymd, const months& dm) noexcept
{
    return ymd + (-dm);
}

constexpr inline year_month_day operator+(const year_month_day& ymd, const years& dy) noexcept
{
    return year_month_day((ymd.year() + dy) , ymd.month() , ymd.day());
}

constexpr inline year_month_day operator+(const years& dy, const year_month_day& ymd) noexcept
{
    return ymd + dy;
}

constexpr inline year_month_day operator-(const year_month_day& ymd, const years& dy) noexcept
{
    return ymd + (-dy);
}

inline year_month_day& year_month_day::operator+=(const months& m) noexcept
{
    *this = *this + m;
    return *this;
}

inline year_month_day& year_month_day::operator-=(const months& m) noexcept
{
    *this = *this - m;
    return *this;
}

inline year_month_day& year_month_day::operator+=(const years& y) noexcept
{
    *this = *this + y;
    return *this;
}

inline year_month_day& year_month_day::operator-=(const years& y) noexcept
{
    *this = *this - y;
    return *this;
}

inline year_month_day::year_month_day(const year_month_day_last& ymdl) noexcept
        : y_(ymdl.year())
        , m_(ymdl.month())
        , d_(ymdl.day())
        {}

inline bool year_month_day::ok() const noexcept
	 {
		 if (!(y_.ok() && m_.ok()))
			 return false;
		 return myDate::day{1} <= d_ && d_ <= myDate::year_month_day_last(y_ , m_ ).day();
	 }



// year_month_day_last impl

constexpr inline bool operator==(const year_month_day_last& x, const year_month_day_last& y) noexcept
{
    return x.year() == y.year() && x.month_day_last() == y.month_day_last();
}

constexpr inline bool operator!=(const year_month_day_last& x, const year_month_day_last& y) noexcept
{
    return !(x == y);
}

constexpr inline bool operator<(const year_month_day_last& x, const year_month_day_last& y) noexcept
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.month_day_last() < y.month_day_last()));
}

constexpr inline bool operator>(const year_month_day_last& x, const year_month_day_last& y) noexcept
{
    return y < x;
}

constexpr inline bool operator<=(const year_month_day_last& x, const year_month_day_last& y) noexcept
{
    return !(y < x);
}

constexpr inline bool operator>=(const year_month_day_last& x, const year_month_day_last& y) noexcept
{
    return !(x < y);
}

inline myDate::year_month_day_last operator+(const year_month_day_last& ymdl, const months& dm) noexcept
{
	year_month_day ymd = year_month_day(ymdl.operator sys_days() + dm);
	return year_month_day_last(ymd.year(), ymd.month());
}

inline myDate::year_month_day_last operator+(const months& dm, const year_month_day_last& ymdl) noexcept
{
    return ymdl + dm;
}

inline myDate::year_month_day_last operator-(const year_month_day_last& ymdl, const months& dm) noexcept
{
    return ymdl + (-dm);
}

constexpr inline myDate::year_month_day_last operator+(const year_month_day_last& ymdl, const years& dy) noexcept
{
    return {ymdl.year()+dy, ymdl.month_day_last()};
}

constexpr inline myDate::year_month_day_last operator+(const years& dy, const year_month_day_last& ymdl) noexcept
{
    return ymdl + dy;
}

constexpr inline myDate::year_month_day_last operator-(const year_month_day_last& ymdl, const years& dy) noexcept
{
    return ymdl + (-dy);
}

inline myDate::year_month_day_last& year_month_day_last::operator+=(const months& m) noexcept
{
    *this = *this + m;
    return *this;
}

inline year_month_day_last& year_month_day_last::operator-=(const months& m) noexcept
{
    *this = *this - m;
    return *this;
}

inline year_month_day_last& year_month_day_last::operator+=(const years& y) noexcept
{
    *this = *this + y;
    return *this;
}

inline year_month_day_last& year_month_day_last::operator-=(const years& y) noexcept
{
    *this = *this - y;
    return *this;
}

constexpr inline year_month_day_last::year_month_day_last(const myDate::year& y, const myDate::month_day_last& mdl) noexcept
    : y_(y)
    , mdl_(mdl)
    {}


constexpr inline myDate::year year_month_day_last::year() const noexcept {return y_;}
constexpr inline myDate::month year_month_day_last::month() const noexcept {return mdl_.month();}
constexpr inline myDate::month_day_last year_month_day_last::month_day_last() const noexcept
{
    return mdl_;
}

inline myDate::day year_month_day_last::day() const noexcept
{
	constexpr const myDate::day d[] =
    {
        myDate::day(31), myDate::day(28), myDate::day(31),
		myDate::day(30), myDate::day(31), myDate::day(30),
		myDate::day(31), myDate::day(31), myDate::day(30),
		myDate::day(31), myDate::day(30), myDate::day(31)
    };
    return month() != feb || !y_.is_leap() ?
        d[static_cast<unsigned>(month()) - 1] : myDate::day{29};
}

inline year_month_day_last::operator sys_days() const noexcept
{
    return myDate::sys_days(year_month_day(year(),month(),day()));
}

inline year_month_day_last::operator local_days() const noexcept
{
    return myDate::local_days(year_month_day(year(),month(),day()));
}

constexpr inline bool year_month_day_last::ok() const noexcept
{
    return y_.ok() && mdl_.ok();
}

*/

} /* namespace myDate */

#endif /* LIBRARIES_MYDATE_MYDATE_H_ */
