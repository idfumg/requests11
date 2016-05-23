#include "session.h"
#include "connection.h"
#include "service.h"
#include "response.h"

namespace crequests {


    namespace {

        bool can_reuse_connection(const request_t& request,
                                  const request_t& last_request) {
            return
                last_request.uri().domain() == request.uri().domain() and
                last_request.uri().port() == request.uri().port() and
                last_request.uri().protocol() == request.uri().protocol();
        }

    } /* anonymous namespace */


    /************************************************************
     * session_impl_t section.
     ************************************************************/


    class session_impl_t {
    public:
        session_impl_t(service_t& service);

    public:
        future_t<response_ptr_t> Send();

        void set_option(const string_t& url);
        void set_option(const url_t& url);
        void set_option(const protocol_t& protocol);
        void set_option(const domain_t& domain);
        void set_option(const port_t& port);
        void set_option(const path_t& path);
        void set_option(const query_t& query);
        void set_option(const params_t& params);
        void set_option(const method_t& method);
        void set_option(const timeout_t& timeout);
        void set_option(const store_timeout_t& store_timeout);
        void set_option(const redirect_t& redirect);
        void set_option(const redirect_count_t& redirect_count);
        void set_option(const gzip_t& gzip);
        void set_option(const headers_t& headers);
        void set_option(const final_callback_t& final_callback);
        void set_option(const data_t& data);
        void set_option(const auth_t& auth);
        void set_option(const keep_alive_t& keep_alive);

        void set_option(string_t&& url);
        void set_option(url_t&& url);
        void set_option(protocol_t&& protocol);
        void set_option(domain_t&& domain);
        void set_option(port_t&& port);
        void set_option(path_t&& path);
        void set_option(query_t&& query);
        void set_option(params_t&& params);
        void set_option(method_t&& method);
        void set_option(timeout_t&& timeout);
        void set_option(store_timeout_t&& store_timeout);
        void set_option(redirect_t&& redirect);
        void set_option(redirect_count_t&& redirect_count);
        void set_option(gzip_t&& gzip);
        void set_option(headers_t&& headers);
        void set_option(final_callback_t&& final_callback);
        void set_option(data_t&& data);
        void set_option(auth_t&& auth);
        void set_option(keep_alive_t&& keep_alive);

        bool is_expired() const;
        void skip_redirects(const response_t& response);
        
    private:
        service_t& service;
        request_t request;
        connection_ptr_t connection;
    };


    session_impl_t::session_impl_t(service_t& service)
        : service(service)
    {
        
    }


    /****************************************************************************
     * Set. Constant reference.
     ***************************************************************************/


    void session_impl_t::set_option(const string_t& url) {
        request.url(url);
    }

    void session_impl_t::set_option(const url_t& url) {
        request.url(url);
    }

    void session_impl_t::set_option(const protocol_t& protocol) {
        request.protocol(protocol);
    }

    void session_impl_t::set_option(const domain_t& domain) {
        request.domain(domain);
    }

    void session_impl_t::set_option(const port_t& port) {
        request.port(port);
    }

    void session_impl_t::set_option(const path_t& path) {
        request.path(path);
    }

    void session_impl_t::set_option(const query_t& query) {
        request.query(query);
    }

    void session_impl_t::set_option(const params_t& params) {
        request.params(params);
    }

    void session_impl_t::set_option(const method_t& method) {
        request.method(method);
    }

    void session_impl_t::set_option(const timeout_t& timeout) {
        request.timeout(timeout);
    }

    void session_impl_t::set_option(const store_timeout_t& store_timeout) {
        request.store_timeout(store_timeout);
    }

    void session_impl_t::set_option(const redirect_t& redirect) {
        request.redirect(redirect);
    }

    void session_impl_t::set_option(const redirect_count_t& redirect_count) {
        request.redirect_count(redirect_count);
    }

    void session_impl_t::set_option(const gzip_t& gzip) {
        request.gzip(gzip);
    }

    void session_impl_t::set_option(const headers_t& headers) {
        request.headers(headers);
    }

    void session_impl_t::set_option(const final_callback_t& final_callback) {
        request.final_callback(final_callback);
    }

    void session_impl_t::set_option(const data_t& data) {
        request.data(data);
    }

    void session_impl_t::set_option(const auth_t& auth) {
        request.auth(auth);
    }

    void session_impl_t::set_option(const keep_alive_t& keep_alive) {
        request.keep_alive(keep_alive);
    }


    /****************************************************************************
     * Set. Rvalue reference.
     ***************************************************************************/


    void session_impl_t::set_option(string_t&& url) {
        request.url(std::move(url));
    }

    void session_impl_t::set_option(url_t&& url) {
        request.url(std::move(url));
    }

    void session_impl_t::set_option(protocol_t&& protocol) {
        request.protocol(std::move(protocol));
    }

    void session_impl_t::set_option(domain_t&& domain) {
        request.domain(std::move(domain));
    }

    void session_impl_t::set_option(port_t&& port) {
        request.port(std::move(port));
    }

    void session_impl_t::set_option(path_t&& path) {
        request.path(std::move(path));
    }

    void session_impl_t::set_option(query_t&& query) {
        request.query(std::move(query));
    }

    void session_impl_t::set_option(params_t&& params) {
        request.params(std::move(params));
    }

    void session_impl_t::set_option(method_t&& method) {
        request.method(std::move(method));
    }

    void session_impl_t::set_option(timeout_t&& timeout) {
        request.timeout(std::move(timeout));
    }

    void session_impl_t::set_option(store_timeout_t&& store_timeout) {
        request.store_timeout(std::move(store_timeout));
    }

    void session_impl_t::set_option(redirect_t&& redirect) {
        request.redirect(std::move(redirect));
    }

    void session_impl_t::set_option(redirect_count_t&& redirect_count) {
        request.redirect_count(std::move(redirect_count));
    }

    void session_impl_t::set_option(gzip_t&& gzip) {
        request.gzip(std::move(gzip));
    }

    void session_impl_t::set_option(headers_t&& headers) {
        request.headers(std::move(headers));
    }

    void session_impl_t::set_option(final_callback_t&& final_callback) {
        request.final_callback(std::move(final_callback));
    }

    void session_impl_t::set_option(data_t&& data) {
        request.data(std::move(data));
    }

    void session_impl_t::set_option(auth_t&& auth) {
        request.auth(std::move(auth));
    }

    void session_impl_t::set_option(keep_alive_t&& keep_alive) {
        request.keep_alive(std::move(keep_alive));
    }


    /****************************************************************************
     * Other functions.
     ***************************************************************************/


    future_t<response_ptr_t> session_impl_t::Send() {
        if (connection and request.cache_redirects())
            skip_redirects(*connection->get().get());
        else
            request.prepare();

        if (not connection or
            not can_reuse_connection(request, connection->get().get()->request())) {
            connection =
                std::make_shared<connection_t>(service, request);
        }
        else {
            auto cookies = request.cookies();
            cookies.update(connection->get().get()->cookies());
            request.cookies(cookies);
            connection =
                std::make_shared<connection_t>(service, request, *connection);
        }

        connection->start();

        return connection->get();
    }

    void session_impl_t::skip_redirects(const response_t& response) {
        const auto resp = response.redirects().find(request);
        if (resp) {
            request.uri(resp->request().uri());
            request.auth(resp->request().auth());
            request.cookies(resp->request().cookies());
            request.prepare();
        }
    }

    bool session_impl_t::is_expired() const {
        return connection and connection->is_expired();
    }


    /************************************************************
     * session_t section.
     ************************************************************/


    session_t::session_t(service_t& service)
        : pimpl {std::make_shared<session_impl_t>(service)}
    {
        
    }

    session_t::session_t(const session_t& session)
        : pimpl {session.pimpl}
    {
        
    }

    session_t::session_t(session_t&& session)
        : pimpl {std::move(session.pimpl)}
    {
        
    }

    session_t::~session_t()
    {
        
    }

    session_t& session_t::operator=(const session_t& session) {
        if (this != &session) {
            pimpl = session.pimpl;
        }

        return *this;
    }


    /****************************************************************************
     * Set. Constant reference.
     ***************************************************************************/


    void session_t::set_option(const string_t& url) {
        pimpl->set_option(url);
    }

    void session_t::set_option(const url_t& url) {
        pimpl->set_option(url);
    }

    void session_t::set_option(const protocol_t& protocol) {
        pimpl->set_option(protocol);
    }

    void session_t::set_option(const domain_t& domain) {
        pimpl->set_option(domain);
    }

    void session_t::set_option(const port_t& port) {
        pimpl->set_option(port);
    }

    void session_t::set_option(const path_t& path) {
        pimpl->set_option(path);
    }

    void session_t::set_option(const query_t& query) {
        pimpl->set_option(query);
    }

    void session_t::set_option(const params_t& params) {
        pimpl->set_option(params);
    }

    void session_t::set_option(const method_t& method) {
        pimpl->set_option(method);
    }

    void session_t::set_option(const timeout_t& timeout) {
        pimpl->set_option(timeout);
    }

    void session_t::set_option(const store_timeout_t& store_timeout) {
        pimpl->set_option(store_timeout);
    }

    void session_t::set_option(const redirect_t& redirect) {
        pimpl->set_option(redirect);
    }

    void session_t::set_option(const redirect_count_t& redirect_count) {
        pimpl->set_option(redirect_count);
    }

    void session_t::set_option(const gzip_t& gzip) {
        pimpl->set_option(gzip);
    }

    void session_t::set_option(const headers_t& headers) {
        pimpl->set_option(headers);
    }

    void session_t::set_option(const final_callback_t& final_callback) {
        pimpl->set_option(final_callback);
    }

    void session_t::set_option(const data_t& data) {
        pimpl->set_option(data);
    }

    void session_t::set_option(const auth_t& auth) {
        pimpl->set_option(auth);
    }

    void session_t::set_option(const keep_alive_t& keep_alive) {
        pimpl->set_option(keep_alive);
    }


    /****************************************************************************
     * Set. Rvalue reference.
     ***************************************************************************/


    void session_t::set_option(string_t&& url) {
        pimpl->set_option(std::move(url));
    }

    void session_t::set_option(url_t&& url) {
        pimpl->set_option(std::move(url));
    }

    void session_t::set_option(protocol_t&& protocol) {
        pimpl->set_option(std::move(protocol));
    }

    void session_t::set_option(domain_t&& domain) {
        pimpl->set_option(std::move(domain));
    }

    void session_t::set_option(port_t&& port) {
        pimpl->set_option(std::move(port));
    }

    void session_t::set_option(path_t&& path) {
        pimpl->set_option(std::move(path));
    }

    void session_t::set_option(query_t&& query) {
        pimpl->set_option(std::move(query));
    }

    void session_t::set_option(params_t&& params) {
        pimpl->set_option(std::move(params));
    }

    void session_t::set_option(method_t&& method) {
        pimpl->set_option(std::move(method));
    }

    void session_t::set_option(timeout_t&& timeout) {
        pimpl->set_option(std::move(timeout));
    }

    void session_t::set_option(store_timeout_t&& store_timeout) {
        pimpl->set_option(std::move(store_timeout));
    }

    void session_t::set_option(redirect_t&& redirect) {
        pimpl->set_option(std::move(redirect));
    }

    void session_t::set_option(redirect_count_t&& redirect_count) {
        pimpl->set_option(std::move(redirect_count));
    }

    void session_t::set_option(gzip_t&& gzip) {
        pimpl->set_option(std::move(gzip));
    }

    void session_t::set_option(headers_t&& headers) {
        pimpl->set_option(std::move(headers));
    }

    void session_t::set_option(final_callback_t&& final_callback) {
        pimpl->set_option(std::move(final_callback));
    }

    void session_t::set_option(data_t&& data) {
        pimpl->set_option(std::move(data));
    }

    void session_t::set_option(auth_t&& auth) {
        pimpl->set_option(std::move(auth));
    }

    void session_t::set_option(keep_alive_t&& keep_alive) {
        pimpl->set_option(std::move(keep_alive));
    }


    /****************************************************************************
     * Http methods.
     ***************************************************************************/


    future_t<response_ptr_t> session_t::AsyncGet() {
        pimpl->set_option(method_t {"GET"});
        return AsyncSend();
    }

    future_t<response_ptr_t> session_t::AsyncPost() {
        pimpl->set_option(method_t {"POST"});
        return AsyncSend();
    }

    future_t<response_ptr_t> session_t::AsyncPut() {
        pimpl->set_option(method_t {"PUT"});
        return AsyncSend();
    }

    future_t<response_ptr_t> session_t::AsyncPatch() {
        pimpl->set_option(method_t {"PATCH"});
        return AsyncSend();
    }

    future_t<response_ptr_t> session_t::AsyncDelete() {
        pimpl->set_option(method_t {"DELETE"});
        return AsyncSend();
    }

    future_t<response_ptr_t> session_t::AsyncHead() {
        pimpl->set_option(method_t {"HEAD"});
        return AsyncSend();
    }

    future_t<response_ptr_t> session_t::AsyncSend() {
        return pimpl->Send();
    }

    response_ptr_t session_t::Get() {
        pimpl->set_option(method_t {"GET"});
        return Send();
    }

    response_ptr_t session_t::Post() {
        pimpl->set_option(method_t {"POST"});
        return Send();
    }

    response_ptr_t session_t::Put() {
        pimpl->set_option(method_t {"PUT"});
        return Send();
    }

    response_ptr_t session_t::Patch() {
        pimpl->set_option(method_t {"PATCH"});
        return Send();
    }

    response_ptr_t session_t::Delete() {
        pimpl->set_option(method_t {"DELETE"});
        return Send();
    }

    response_ptr_t session_t::Head() {
        pimpl->set_option(method_t {"HEAD"});
        return Send();
    }

    response_ptr_t session_t::Send() {
        return pimpl->Send().get();
    }


    /****************************************************************************
     * Other functions.
     ***************************************************************************/


    bool session_t::is_expired() const {
        return pimpl->is_expired();
    }


} /* namespace crequests */
