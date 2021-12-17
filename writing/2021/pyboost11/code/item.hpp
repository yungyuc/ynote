#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

struct ItemBpy;

/* To be wrapped by pybind11 */
struct ItemPyb
{
    ItemPyb(int v) : m_value(v) {}
    ItemPyb() = default;
    ItemPyb(ItemPyb const & other) : m_value(other.m_value) {}
    ItemPyb & operator=(ItemPyb const & other) { m_value = other.m_value; return *this; }

    int value() const { return m_value; }
    void set_value(int v) { m_value = v; }

    intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

    void take_pyb(ItemPyb const & other);
    void take_bpy(ItemBpy const & other);

    ItemPyb make_pyb();
    ItemBpy make_bpy();

private:

    int m_value = 0;
};

/* To be wrapped by boost.python */
struct ItemBpy
{
    ItemBpy(int v) : m_value(v) {}
    ItemBpy() = default;
    ItemBpy(ItemBpy const & other) : m_value(other.m_value) {}
    ItemBpy & operator=(ItemBpy const & other) { m_value = other.m_value; return *this; }

    int value() const { return m_value; }
    void set_value(int v) { m_value = v; }

    intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

    void take_pyb(ItemPyb const & other);
    void take_bpy(ItemBpy const & other);

    ItemPyb make_pyb();
    ItemBpy make_bpy();

private:

    int m_value = 0;
};

inline void ItemBpy::take_pyb(ItemPyb const & other) { m_value = other.value(); }
inline void ItemBpy::take_bpy(ItemBpy const & other) { m_value = other.value(); }

inline ItemPyb ItemBpy::make_pyb() { return ItemPyb(value()); }
inline ItemBpy ItemBpy::make_bpy() { return ItemBpy(value()); }

inline void ItemPyb::take_pyb(ItemPyb const & other) { m_value = other.value(); }
inline void ItemPyb::take_bpy(ItemBpy const & other) { m_value = other.value(); }

inline ItemPyb ItemPyb::make_pyb() { return ItemPyb(value()); }
inline ItemBpy ItemPyb::make_bpy() { return ItemBpy(value()); }

struct ItemBpyBS;

/* To be wrapped by pybind11 */
struct ItemPybSS : std::enable_shared_from_this<ItemPybSS>
{
private:

    struct ctor_passkey {};

public:

    template < typename ... Args >
    static std::shared_ptr<ItemPybSS> make(Args && ... args)
    {
        return std::make_shared<ItemPybSS>(std::forward<Args>(args) ..., ctor_passkey());
    }

    ItemPybSS(int v, ctor_passkey const &) : m_value(v) {}
    ItemPybSS() = delete;
    ItemPybSS(ItemPybSS const &) = delete;
    ItemPybSS & operator=(ItemPybSS const &) = delete;

    int value() const { return m_value; }
    void set_value(int v) { m_value = v; }

    intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

    void take_pybss(ItemPybSS const & other);
    void take_bpybs(ItemBpyBS const & other);

    std::shared_ptr<ItemPybSS> make_pybss();
    boost::shared_ptr<ItemBpyBS> make_bpybs();

private:

    int m_value = 0;
};

/* To be wrapped by boost.python */
struct ItemBpyBS : boost::enable_shared_from_this<ItemBpyBS>
{
private:

    struct ctor_passkey {};

public:

    template < typename ... Args >
    static boost::shared_ptr<ItemBpyBS> make(Args && ... args)
    {
        return boost::make_shared<ItemBpyBS>(std::forward<Args>(args) ..., ctor_passkey());
    }

    ItemBpyBS(int v, ctor_passkey const &) : m_value(v) {}
    ItemBpyBS() = delete;
    ItemBpyBS(ItemBpyBS const &) = delete;
    ItemBpyBS & operator=(ItemBpyBS const &) = delete;

    int value() const { return m_value; }
    void set_value(int v) { m_value = v; }

    intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

    void take_pybss(ItemPybSS const & other);
    void take_bpybs(ItemBpyBS const & other);

    std::shared_ptr<ItemPybSS> make_pybss();
    boost::shared_ptr<ItemBpyBS> make_bpybs();

private:

    int m_value = 0;
};

inline void ItemBpyBS::take_pybss(ItemPybSS const & other) { m_value = other.value(); }
inline void ItemBpyBS::take_bpybs(ItemBpyBS const & other) { m_value = other.value(); }

inline std::shared_ptr<ItemPybSS> ItemBpyBS::make_pybss() { return ItemPybSS::make(value()); }
inline boost::shared_ptr<ItemBpyBS> ItemBpyBS::make_bpybs() { return ItemBpyBS::make(value()); }

inline void ItemPybSS::take_pybss(ItemPybSS const & other) { m_value = other.value(); }
inline void ItemPybSS::take_bpybs(ItemBpyBS const & other) { m_value = other.value(); }

inline std::shared_ptr<ItemPybSS> ItemPybSS::make_pybss() { return ItemPybSS::make(value()); }
inline boost::shared_ptr<ItemBpyBS> ItemPybSS::make_bpybs() { return ItemBpyBS::make(value()); }

/* To be wrapped by pybind11 */
struct ContainerPyb
{
    ContainerPyb
    (
        ItemPyb const & pyb, ItemBpy const & bpy
      , std::shared_ptr<ItemPybSS> const & pybss
      , boost::shared_ptr<ItemBpyBS> const & bpybs
    ) : m_pyb(pyb), m_bpy(bpy), m_pybss(pybss), m_bpybs(bpybs) {}

    ItemPyb const & pyb() const { return m_pyb; }
    ItemBpy const & bpy() const { return m_bpy; }

    std::shared_ptr<ItemPybSS> const & pybss() const { return m_pybss; }
    boost::shared_ptr<ItemBpyBS> const & bpybs() const { return m_bpybs; }

    void take_pyb(ItemPyb const & other) { m_pyb.set_value(other.value()); }
    void take_bpy(ItemBpy const & other) { m_bpy.set_value(other.value()); }

    void set_pybss(std::shared_ptr<ItemPybSS> const & other) { m_pybss = other; }
    void set_bpybs(boost::shared_ptr<ItemBpyBS> const & other) { m_bpybs = other; }

    void set_pybss_ref(ItemPybSS & other) { m_pybss = other.shared_from_this(); }
    void set_bpybs_ref(ItemBpyBS & other) { m_bpybs = other.shared_from_this(); }

    std::string overload_pyb(ItemPyb const &) const { return "overload_pyb_noss"; }
    std::string overload_pyb(std::shared_ptr<ItemPybSS> const &) const { return "overload_pyb_ss"; }

    std::string overload_bpy(ItemBpy const &) const { return "overload_bpy_nobs"; }
    std::string overload_bpy(boost::shared_ptr<ItemBpyBS> const &) const { return "overload_bpy_bs"; }

private:

    ItemPyb m_pyb;
    ItemBpy m_bpy;
    std::shared_ptr<ItemPybSS> m_pybss;
    boost::shared_ptr<ItemBpyBS> m_bpybs;
};

/* To be wrapped by boost.python */
struct ContainerBpy
{
    ContainerBpy
    (
        ItemPyb const & pyb, ItemBpy const & bpy
      , std::shared_ptr<ItemPybSS> const & pybss
      , boost::shared_ptr<ItemBpyBS> const & bpybs
    ) : m_pyb(pyb), m_bpy(bpy), m_pybss(pybss), m_bpybs(bpybs) {}

    ItemPyb const & pyb() const { return m_pyb; }
    ItemBpy const & bpy() const { return m_bpy; }

    std::shared_ptr<ItemPybSS> const & pybss() const { return m_pybss; }
    boost::shared_ptr<ItemBpyBS> const & bpybs() const { return m_bpybs; }

    void take_pyb(ItemPyb const & other) { m_pyb.set_value(other.value()); }
    void take_bpy(ItemBpy const & other) { m_bpy.set_value(other.value()); }

    void set_pybss(std::shared_ptr<ItemPybSS> const & other) { m_pybss = other; }
    void set_bpybs(boost::shared_ptr<ItemBpyBS> const & other) { m_bpybs = other; }

    void set_pybss_ref(ItemPybSS & other) { m_pybss = other.shared_from_this(); }
    void set_bpybs_ref(ItemBpyBS & other) { m_bpybs = other.shared_from_this(); }

    std::string overload_pyb(ItemPyb const &) const { return "overload_pyb_noss"; }
    std::string overload_pyb(std::shared_ptr<ItemPybSS> const &) const { return "overload_pyb_ss"; }

    std::string overload_bpy(ItemBpy const &) const { return "overload_bpy_nobs"; }
    std::string overload_bpy(boost::shared_ptr<ItemBpyBS> const &) const { return "overload_bpy_bs"; }

private:

    ItemPyb m_pyb;
    ItemBpy m_bpy;
    std::shared_ptr<ItemPybSS> m_pybss;
    boost::shared_ptr<ItemBpyBS> m_bpybs;
};

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
