#include "gnmi/gnmi.pb-c.c"

void *gnmi_acton_malloc_cb(void *mem_user_data, size_t size) {
    return acton_malloc(size);
}

void gnmi_acton_free_cb(void *mem_user_data, void *ptr) {
    return acton_free(ptr);
}
struct ProtobufCAllocator gnmi_acton_alloc = { gnmi_acton_malloc_cb, gnmi_acton_free_cb, NULL };


void gnmiQ_protoQ___ext_init__() {
   // NOP
}

int path_acton_to_proto(Gnmi__Path *proto_path, gnmiQ_protoQ_Path acton_path) {
    proto_path->origin = (char*)fromB_str(acton_path->origin);
    proto_path->target = (char*)fromB_str(acton_path->target);

    size_t n_elem = acton_path->elems->length; 
    gnmiQ_protoQ_PathElem *acton_elems = (gnmiQ_protoQ_PathElem*)(acton_path->elems->data); 

    proto_path->n_elem = n_elem;
    proto_path->elem = acton_calloc(n_elem, sizeof(Gnmi__PathElem*));
    
    for (size_t i = 0; i < n_elem; ++i) {
        Gnmi__PathElem *elem = proto_path->elem[i] = acton_malloc(sizeof(Gnmi__PathElem));
        gnmi__path_elem__init(elem);
        proto_path->elem[i]->name = (char*)fromB_str(acton_elems[i]->name);
    
        size_t n_key = acton_elems[i]->keys->length;
        elem->n_key = n_key;
        elem->key = acton_calloc(n_key, sizeof(Gnmi__PathElem__KeyEntry*));
        B_tuple *acton_keys = (B_tuple*)acton_elems[i]->keys->data;
    
        for (size_t j = 0; j < n_key; ++j) {
            elem->key[j] = acton_malloc(sizeof(Gnmi__PathElem__KeyEntry));
            gnmi__path_elem__key_entry__init(elem->key[j]);
            elem->key[j]->key = (char*)fromB_str(acton_keys[j]->components[0]);
            elem->key[j]->value = (char*)fromB_str(acton_keys[j]->components[1]);
        }
    }
    return 0;
}

gnmiQ_protoQ_Path path_proto_to_acton(Gnmi__Path *proto_path) {
    size_t n_elem = proto_path->n_elem;
    B_list elems = B_listD_new(n_elem);
    B_Sequence elem_wit = (B_Sequence)B_SequenceD_listG_witness;

    for (size_t i = 0; i < n_elem; ++i) {
        size_t n_key = proto_path->elem[i]->n_key;
        B_list keys = B_listD_new(n_key);
        B_Sequence key_wit = (B_Sequence)B_SequenceD_listG_witness;

        for (size_t j = 0; j < n_key; ++j) {
            char *key = proto_path->elem[i]->key[j]->key;
            char *value = proto_path->elem[i]->key[j]->value;
            B_tuple pair = $NEWTUPLE(2, to$str(key), to$str(value));
            key_wit->$class->append(key_wit, keys, pair);
        }
        elem_wit->$class->append(elem_wit, elems, gnmiQ_protoQ_PathElemG_new(to$str(proto_path->elem[i]->name), keys));
    }

    B_str origin = to$str(proto_path->origin);
    B_str target = to$str(proto_path->target);

    gnmiQ_protoQ_Path acton_path = gnmiQ_protoQ_PathG_new(elems, origin, target);

    return acton_path;
}

B_bytes gnmiQ_protoQ_pack_SubscribeRequest(gnmiQ_protoQ_SubscribeRequest acton_subscribe_request) {
    Gnmi__SubscribeRequest subscribe_request;

    gnmi__subscribe_request__init(&subscribe_request);

    // no extension support for now
    subscribe_request.n_extension = 0;

    if (acton_subscribe_request->subscribe) {
        gnmiQ_protoQ_SubscriptionList acton_subscription_list = acton_subscribe_request->subscribe;
        subscribe_request.request_case = GNMI__SUBSCRIBE_REQUEST__REQUEST_SUBSCRIBE;
        subscribe_request.subscribe = acton_malloc(sizeof(Gnmi__SubscriptionList));
        //initialize subscription list
        gnmi__subscription_list__init(subscribe_request.subscribe);

        // set prefix path
        subscribe_request.subscribe->prefix = acton_malloc(sizeof(Gnmi__Path));
        gnmi__path__init(subscribe_request.subscribe->prefix);
        path_acton_to_proto(subscribe_request.subscribe->prefix, acton_subscription_list->prefix);

        //various scalar fields
        subscribe_request.subscribe->qos = acton_malloc(sizeof(Gnmi__QOSMarking));
        gnmi__qosmarking__init(subscribe_request.subscribe->qos);
        subscribe_request.subscribe->qos->marking = fromB_u32(acton_subscription_list->qos);
        subscribe_request.subscribe->mode = from$int(acton_subscription_list->mode);
        subscribe_request.subscribe->allow_aggregation = fromB_bool(acton_subscription_list->allow_aggregation);
        subscribe_request.subscribe->encoding = from$int(acton_subscription_list->encoding);
        subscribe_request.subscribe->updates_only = fromB_bool(acton_subscription_list->updates_only);

        // subscriptions
        size_t n_subscription = acton_subscription_list->subscriptions->length;
        gnmiQ_protoQ_Subscription *acton_subscriptions = (gnmiQ_protoQ_Subscription*)acton_subscription_list->subscriptions->data;
        subscribe_request.subscribe->n_subscription = n_subscription;
        Gnmi__Subscription **subscriptions = subscribe_request.subscribe->subscription = acton_calloc(n_subscription, sizeof(Gnmi__Subscription*));

        for (size_t i = 0; i < n_subscription; ++i) {
            subscriptions[i] = acton_malloc(sizeof(Gnmi__Subscription));
            gnmi__subscription__init(subscriptions[i]);

            subscriptions[i]->path = acton_malloc(sizeof(Gnmi__Path));
            gnmi__path__init(subscriptions[i]->path);
            path_acton_to_proto(subscriptions[i]->path, acton_subscriptions[i]->path);

            subscriptions[i]->mode = from$int(acton_subscriptions[i]->mode);
            subscriptions[i]->sample_interval = fromB_u64(acton_subscriptions[i]->sample_interval);
            subscriptions[i]->suppress_redundant = fromB_bool(acton_subscriptions[i]->suppress_redundant);
            subscriptions[i]->heartbeat_interval = fromB_u64(acton_subscriptions[i]->heartbeat_interval);
        }

        // models
        size_t n_use_models = acton_subscription_list->use_models->length;
        gnmiQ_protoQ_ModelData *acton_use_models = (gnmiQ_protoQ_ModelData*)acton_subscription_list->use_models->data;
        subscribe_request.subscribe->n_use_models = n_use_models;
        Gnmi__ModelData **use_models = subscribe_request.subscribe->use_models = acton_calloc(n_use_models, sizeof(Gnmi__ModelData*));

        for (size_t i = 0; i < n_use_models; ++i) {
            use_models[i] = acton_malloc(sizeof(Gnmi__ModelData));
            gnmi__model_data__init(use_models[i]);

            use_models[i]->name = (char*)fromB_str(acton_use_models[i]->name);
            use_models[i]->organization = (char*)fromB_str(acton_use_models[i]->organization);
            use_models[i]->version = (char*)fromB_str(acton_use_models[i]->version);
        }
    }
    else {
        subscribe_request.request_case = GNMI__SUBSCRIBE_REQUEST__REQUEST_POLL;
        subscribe_request.poll = acton_malloc(sizeof(Gnmi__Poll));
        gnmi__poll__init(subscribe_request.poll);
    }

    size_t buffer_size = gnmi__subscribe_request__get_packed_size(&subscribe_request);

    uint8_t *buffer = acton_malloc(buffer_size);
    gnmi__subscribe_request__pack(&subscribe_request, buffer);

    B_bytes ret = to$bytesD_len((char*)buffer, buffer_size);

    return ret;
}

gnmiQ_protoQ_TypedValue typed_val_proto_to_acton(Gnmi__TypedValue* val) {
    gnmiQ_protoQ_TypedValue typed_val;
    ProtobufCBinaryData bytes_val;
    B_bytes acton_bytes_val;
    B_bytes b_bytes_val;
    B_str b_str_val;
    B_i64 b_i64_val;
    B_u64 b_u64_val;
    B_bool b_bool_val;
    B_str string_val;
    B_float b_float_val;
    size_t n_element;
    B_list list_val;
    B_Sequence leaflist_wit;
    B_str type_url;
    gnmiQ_protoQ_Any any_val;
    B_dict json_val;
    B_str ascii_val;
    B_bytes proto_val;

    switch(val->value_case) {
        case GNMI__TYPED_VALUE__VALUE__NOT_SET:
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_NotSetValueG_new();
            break;
        case GNMI__TYPED_VALUE__VALUE_STRING_VAL:
            b_str_val = to$str(val->string_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_StringValueG_new(b_str_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_INT_VAL:
            b_i64_val = toB_i64(val->int_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_IntValueG_new(b_i64_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_UINT_VAL:
            b_u64_val = toB_u64(val->uint_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_UIntValueG_new(b_u64_val);
            break;
            break;
        case GNMI__TYPED_VALUE__VALUE_BOOL_VAL:
            b_bool_val = toB_bool(val->bool_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_BoolValueG_new(b_bool_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_BYTES_VAL:
            bytes_val = val->bytes_val;
            b_bytes_val = to$bytesD_len((char*)bytes_val.data, bytes_val.len);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_BytesValueG_new(b_bytes_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_FLOAT_VAL:
            break;
        case GNMI__TYPED_VALUE__VALUE_DOUBLE_VAL:
            b_float_val = toB_float(val->double_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_DoubleValueG_new(b_float_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_DECIMAL_VAL:
            break;
        case GNMI__TYPED_VALUE__VALUE_LEAFLIST_VAL:
            n_element = val->leaflist_val->n_element;
            list_val = B_listD_new(n_element);

            leaflist_wit = (B_Sequence)B_SequenceD_listG_witness;
            for (size_t i = 0; i < n_element; ++i) {
                Gnmi__TypedValue *leaf_val = val->leaflist_val->element[i];
                gnmiQ_protoQ_TypedValue leaf_typed_val = typed_val_proto_to_acton(leaf_val);
                leaflist_wit->$class->append(leaflist_wit, list_val, leaf_typed_val);
            }
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_LeafListValueG_new(list_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_ANY_VAL:
            type_url = to$str(val->any_val->type_url);
            bytes_val = val->any_val->value;
            acton_bytes_val = to$bytesD_len((char*)bytes_val.data, bytes_val.len);
            any_val = gnmiQ_protoQ_AnyG_new(type_url, acton_bytes_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_AnyValueG_new(any_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_JSON_VAL:
            bytes_val = val->json_val;
            acton_bytes_val = to$bytesD_len((char*)bytes_val.data, bytes_val.len);
            string_val = to$str((char*)acton_bytes_val->str);
            json_val = jsonQ_decode(string_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_JsonValueG_new(json_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_JSON_IETF_VAL:
            bytes_val = val->json_ietf_val;
            acton_bytes_val = to$bytesD_len((char*)bytes_val.data, bytes_val.len);
            string_val = to$str((char*)acton_bytes_val->str);
            json_val = jsonQ_decode(string_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_JsonIetfValueG_new(json_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_ASCII_VAL:
            B_str ascii_val = to$str(val->ascii_val);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_AsciiValueG_new(ascii_val);
            break;
        case GNMI__TYPED_VALUE__VALUE_PROTO_BYTES:
            bytes_val = val->proto_bytes;
            proto_val = to$bytesD_len((char*)bytes_val.data, bytes_val.len);
            typed_val = (gnmiQ_protoQ_TypedValue)gnmiQ_protoQ_ProtoBytesValueG_new(proto_val);
            break;
    }

    return typed_val;
}

gnmiQ_protoQ_SubscribeResponse gnmiQ_protoQ_unpack_SubscribeResponse(B_bytes data) {

    Gnmi__SubscribeResponse *subscribe_response = gnmi__subscribe_response__unpack(&gnmi_acton_alloc, data->nbytes, data->str);

    gnmiQ_protoQ_Notification acton_notif = NULL;
    B_bool sync_response = toB_bool(false);

    // skipping extensions for now

    switch (subscribe_response->response_case) {
        case GNMI__SUBSCRIBE_RESPONSE__RESPONSE__NOT_SET:
            break;
        case GNMI__SUBSCRIBE_RESPONSE__RESPONSE_UPDATE:
            Gnmi__Notification *proto_notif = subscribe_response->update;

            B_i64 timestamp = toB_i64(proto_notif->timestamp);
            gnmiQ_protoQ_Path prefix = path_proto_to_acton(proto_notif->prefix);
            B_bool atomic = toB_bool(proto_notif->atomic);

	    size_t n_update = proto_notif->n_update;
            B_list updates = B_listD_new(n_update);
            B_Sequence update_wit = (B_Sequence)B_SequenceD_listG_witness;
            for (size_t i = 0; i < n_update; ++i) {
                Gnmi__Update *proto_update = proto_notif->update[i];
                gnmiQ_protoQ_Path update_path = path_proto_to_acton(proto_update->path);
                gnmiQ_protoQ_TypedValue typed_val = typed_val_proto_to_acton(proto_update->val);
                gnmiQ_protoQ_Update acton_update = gnmiQ_protoQ_UpdateG_new(update_path, typed_val, toB_u32(proto_update->duplicates));
                update_wit->$class->append(update_wit, updates, acton_update);
	    }

            size_t n_delete = proto_notif->n_delete_;
            B_list deletes = B_listD_new(n_delete);
            B_Sequence delete_wit = (B_Sequence)B_SequenceD_listG_witness;
            for (size_t i = 0; i < n_delete; ++i) {
	    }

            acton_notif = gnmiQ_protoQ_NotificationG_new(timestamp, prefix, updates, deletes, atomic);
            break;
        case GNMI__SUBSCRIBE_RESPONSE__RESPONSE_SYNC_RESPONSE:
            break;
        case GNMI__SUBSCRIBE_RESPONSE__RESPONSE_ERROR:
            // deprecated case
            break;
    }
    gnmiQ_protoQ_SubscribeResponse response = gnmiQ_protoQ_SubscribeResponseG_new(to$int(subscribe_response->response_case), acton_notif, sync_response);

    return response;
}

