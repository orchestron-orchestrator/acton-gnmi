#include <gnmi/gnmi.pb-c.h>

void gnmiQ_protoQ___ext_init__() {
   // NOP
}

B_bytes gnmiQ_protoQ_pack_SubscribeRequest(gnmiQ_protoQ_SubscribeRequest) {
    Gnmi__SubscribeRequest message;

    gnmi__subscribe_request__init(&message);

    size_t buffer_size = gnmi__subscribe_request__get_packed_size(&message);

    uint8_t *buffer = acton_malloc(buffer_size);
    gnmi__subscribe_request__pack(&message, buffer);

    B_bytes ret = to$bytesD_len((char*)buffer, buffer_size);

    return ret;
}
