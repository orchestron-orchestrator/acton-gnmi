#include "gnmi/gnmi.pb-c.c"

void gnmiQ_protoQ___ext_init__() {
   // NOP
}

int copy_path_data(Gnmi__Path *proto_path, gnmiQ_protoQ_Path acton_path) {
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
        B_tuple *acton_keys = acton_elems[i]->keys->data;
    
        for (size_t i = 0; i < n_key; ++i) {
            elem->key[i] = acton_malloc(sizeof(Gnmi__PathElem__KeyEntry));
            gnmi__path_elem__key_entry__init(elem->key[i]);
            elem->key[i]->key = (char*)fromB_str(acton_keys[i]->components[0]);
            elem->key[i]->value = (char*)fromB_str(acton_keys[i]->components[1]);
        }
    }
    return 0;
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
        copy_path_data(subscribe_request.subscribe->prefix, acton_subscription_list->prefix);

        //various scalar fields
        subscribe_request.subscribe->qos = acton_malloc(sizeof(Gnmi__QOSMarking));
        subscribe_request.subscribe->mode = from$int(acton_subscription_list->mode);
        subscribe_request.subscribe->allow_aggregation = fromB_bool(acton_subscription_list->allow_aggregation);
        subscribe_request.subscribe->encoding = from$int(acton_subscription_list->encoding);
        subscribe_request.subscribe->updates_only = fromB_bool(acton_subscription_list->updates_only);

        // subscriptions
        size_t n_subscription = acton_subscription_list->subscriptions->length;
        gnmiQ_protoQ_Subscription *acton_subscriptions = acton_subscription_list->subscriptions->data;
        subscribe_request.subscribe->n_subscription = n_subscription;
        Gnmi__Subscription **subscriptions = subscribe_request.subscribe->subscription = acton_calloc(n_subscription, sizeof(Gnmi__Subscription*));

        for (size_t i = 0; i < n_subscription; ++i) {
            subscriptions[i] = acton_malloc(sizeof(Gnmi__Subscription));
            gnmi__subscription__init(subscriptions[i]);

            subscriptions[i]->path = acton_malloc(sizeof(Gnmi__Path));
            gnmi__path__init(subscriptions[i]->path);
            copy_path_data(subscriptions[i]->path, acton_subscriptions[i]->path);

            subscriptions[i]->mode = from$int(acton_subscriptions[i]->mode);
            subscriptions[i]->sample_interval = fromB_u64(acton_subscriptions[i]->sample_interval);
            subscriptions[i]->suppress_redundant = fromB_bool(acton_subscriptions[i]->suppress_redundant);
            subscriptions[i]->heartbeat_interval = fromB_u64(acton_subscriptions[i]->heartbeat_interval);
        }

        // models
        size_t n_use_models = acton_subscription_list->use_models->length;
        gnmiQ_protoQ_ModelData *acton_use_models = acton_subscription_list->use_models->data;
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
