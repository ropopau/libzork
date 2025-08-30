#include <libzork/store/store.hh>

#include "exceptions.hh"
#include "store/store_impl.hh"

namespace libzork::store
{

    std::unique_ptr<Store> make_store()
    {
        StoreImpl a = StoreImpl();
        return std::make_unique<StoreImpl>(a);
    }

} // namespace libzork::store
