#ifndef XTENSOR_IO_GCS_HANDLER_HPP
#define XTENSOR_IO_GCS_HANDLER_HPP

#include "xtensor/xarray.hpp"
#include "xtensor/xexpression.hpp"
#include "google/cloud/storage/client.h"

namespace gcs = google::cloud::storage;

namespace xt
{
    struct xio_gcs_config
    {
        gcs::Client client;
        std::string bucket;
    };

    template <class C>
    class xio_gcs_handler
    {
    public:
        using io_config = xio_gcs_config;

        xio_gcs_handler();

        template <class E>
        void write(const xexpression<E>& expression, const std::string& path, xfile_dirty dirty);

        template <class ET>
        void read(ET& array, const std::string& path);

        void configure(const C& format_config, const xio_gcs_config& io_config);
        void configure_io(const xio_gcs_config& io_config);

    private:

        C m_format_config;
        gcs::Client m_client;
        std::string m_bucket;
    };

    template <class C>
    xio_gcs_handler<C>::xio_gcs_handler()
        : m_client(gcs::ClientOptions((gcs::oauth2::CreateAnonymousCredentials())))
    {
    }

    template <class C>
    template <class E>
    inline void xio_gcs_handler<C>::write(const xexpression<E>& expression, const std::string& path, xfile_dirty dirty)
    {
        if (m_format_config.will_dump(dirty))
        {
            auto writer = m_client.WriteObject(m_bucket, path);
            dump_file(writer, expression, m_format_config);
        }
    }

    template <class C>
    template <class ET>
    inline void xio_gcs_handler<C>::read(ET& array, const std::string& path)
    {
        auto reader = m_client.ReadObject(m_bucket, path);
        load_file<ET>(reader, array, m_format_config);
    }

    template <class C>
    inline void xio_gcs_handler<C>::configure(const C& format_config, const xio_gcs_config& io_config)
    {
        m_format_config = format_config;
        m_client = io_config.client;
        m_bucket = io_config.bucket;
    }

    template <class C>
    inline void xio_gcs_handler<C>::configure_io(const xio_gcs_config& io_config)
    {
        m_client = io_config.client;
        m_bucket = io_config.bucket;
    }

}

#endif
