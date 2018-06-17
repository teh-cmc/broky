# Broky

*Broky* is a low-latency push/subscribe message broker:
- In-memory only, no durability/persistence
- Very low latency
- Ephemeral topics automatically created & destroyed by subscribers (*fire and forget*)
- Multiplexer/Fan-in (external producers pushing messages into *Broky*)
- Demultiplexer/Fan-out (*Broky* pushing messages into external subscribers)
- Simple & mechanically sympathetic design and API

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [Data model](#data-model)
- [API](#api)
- [Single node](#single-node)
  - [Sharding model](#sharding-model)
- [Clustering](#clustering)
  - [Sharding model](#sharding-model-1)
- [Replication model](#replication-model)
- [Ops model](#ops-model)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## Data model

```
struct msg {
    /* mandatory */
    key_partition []byte
    key []byte
    payload []byte

    /* optional */
    ts_producer time_t
    ts_broker time_t
    uid ksuid
}
```

The contents of `key_partition`, `key` and `payload` are entirely left to the end-user and are completely opaque to *Broky*.

- The contents of `key_partition` are used internally forsharding, both at the [cluster level](#sharding-model) and down to the [single-node level](#sharding-model-1). See the relevant sections for details.
- `key` is used for subscription matching.

TODO(cmc): Optional fields

TODO(cmc): FlatBuffers

## API

TODO(cmc): Stream multiplexing

TODO(cmc): Push, Subscribe

TODO(cmc): Raw TCP + FlatBuffers, keep it dumb

## Single node

Each node in the cluster runs `nb_cpu_cores` IO event loops (via *libuv*):
- 1 loop (the *master thread*) is dedicated to listening for incoming connections and forwarding accepted TCP streams to *worker threads*.
- `MAX(nb_cpu_cores-1, 1)` loops (the *worker threads*) handle incoming requests for each TCP stream and do the actual work of multiplexing incoming messages and demultiplexing outgoing messages.

### Sharding model

Each worker thread 
Each *worker thread* is responsible 

## Clustering

The sharding and replication models 

Each node in the cluster is part of a global hashring (the *token ring*) and is responsible for some *token ranges* (or *vbuckets*) of that ring, a la Cassandra.

### Sharding model

`hash(msg.key_partition)`

## Replication model

TODO(cmc): Something like Kafka's ISR model I guess.

## Ops model

*Broky* is designed with operational simplicity in mind:
- No gazillions of intertwined knobs, only the bare essentials
- Pure good old C11, no runtime, no VM, no surprises
- Deployable as a single static binary
- Internal behavior observable via Prometheus probes
