#ifndef XTENSOR_IO_DISK_HANDLER_HPP
#define XTENSOR_IO_DISK_HANDLER_HPP

#include <xtensor/xarray.hpp>
#include <xtensor/xexpression.hpp>

namespace xt
{
    struct xio_disk_config
    {
    };

    template <class C>
    class xio_disk_handler
    {
    public:
        using io_config = xio_disk_config;

        template <class E>
        void write(const xexpression<E>& expression, const std::string& path, xfile_dirty dirty);

        template <class ET>
        void read(ET& array, const std::string& path);

        void configure(const C& format_config, const xio_disk_config& io_config);
        void configure_io(const xio_disk_config& io_config);

    private:

        C m_format_config;
    };

    template <class C>
    template <class E>
    inline void xio_disk_handler<C>::write(const xexpression<E>& expression, const std::string& path, xfile_dirty dirty)
    {
        if (m_format_config.will_dump(dirty))
        {
            std::ofstream out_file(path, std::ofstream::binary);
            if (out_file.is_open())
            {
                dump_file(out_file, expression, m_format_config);
            }
            else
            {
                XTENSOR_THROW(std::runtime_error, "write: failed to open file " + path);
            }
        }
    }

    template <class C>
    template <class ET>
    inline void xio_disk_handler<C>::read(ET& array, const std::string& path)
    {
        std::ifstream in_file(path, std::ifstream::binary);
        if (in_file.is_open())
        {
            load_file<ET>(in_file, array, m_format_config);
        }
        else
        {
            XTENSOR_THROW(std::runtime_error, "read: failed to open file " + path);
        }
    }

    template <class C>
    inline void xio_disk_handler<C>::configure(const C& format_config, const xio_disk_config& io_config)
    {
        m_format_config = format_config;
    }

    template <class C>
    inline void xio_disk_handler<C>::configure_io(const xio_disk_config& io_config)
    {
    }

}

#endif
