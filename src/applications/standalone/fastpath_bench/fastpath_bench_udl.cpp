#include <cascade/user_defined_logic_interface.hpp>
#include <iostream>

namespace derecho{
namespace cascade{

#define MY_UUID     "909343a0-6c50-11ee-afcd-0242ac110003"
#define MY_DESC     "FastPath Benchmarking"

std::string get_uuid() {
    return MY_UUID;
}

std::string get_description() {
    return MY_DESC;
}

class FastPathBenchOCDPO: public OffCriticalDataPathObserver {
    virtual void operator () (const node_id_t sender,
                              const std::string& key_string,
                              const uint32_t prefix_length,
                              persistent::version_t version,
                              const mutils::ByteRepresentable* const value_ptr,
                              const std::unordered_map<std::string,bool>& outputs,
                              ICascadeContext* ctxt,
                              uint32_t worker_id) override {
        // timestamp
        const ObjectWithStringKey* obj = dynamic_cast<const ObjectWithStringKey*>(value_ptr);
        TimestampLogger::log(8848,0,obj->get_message_id());
    }

    static std::shared_ptr<OffCriticalDataPathObserver> ocdpo_ptr;
public:
    static void initialize() {
        if(!ocdpo_ptr) {
            ocdpo_ptr = std::make_shared<FastPathBenchOCDPO>();
        }
    }
    static auto get() {
        return ocdpo_ptr;
    }
};

std::shared_ptr<OffCriticalDataPathObserver> FastPathBenchOCDPO::ocdpo_ptr;

void initialize(ICascadeContext* ctxt) {
    FastPathBenchOCDPO::initialize();
}

std::shared_ptr<OffCriticalDataPathObserver> get_observer(
        ICascadeContext*,const nlohmann::json&) {
    return FastPathBenchOCDPO::get();
}

void release(ICascadeContext* ctxt) {
    // nothing to release
    return;
}

} // namespace cascade
} // namespace derecho
