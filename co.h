#ifndef __TINY_CO_H__
#define __TINY_CO_H__

typedef enum
{
    _CO_STATE_INIT,
    _CO_STATE_RUN,
    _CO_STATE_YIELD,
    _CO_STATE_FIN
} _CoState;

struct Co;

typedef int (*CoTask)(struct Co *co, void *data);

typedef struct Co
{
    _CoState state;
    CoTask func;
    void *data;
    int label;
} Co;

#define CO_BEGIN(co)     \
    switch ((co)->label) \
    {                    \
    case 0:
#define CO_YIELD(co, value)     \
    do                          \
    {                           \
        (co)->label = __LINE__; \
        (co)->state = _CO_STATE_YIELD; \
        return (value);         \
    case __LINE__:;             \
    } while (0)
#define CO_END(co)        \
    (co)->state = _CO_STATE_FIN; \
    }

void co_init(Co *co, CoTask func, void *data)
{
    co->state = _CO_STATE_INIT;
    co->func = func;
    co->data = data;
    co->label = 0;
}

int co_next(Co *co)
{
    if (co->state == _CO_STATE_FIN)
    {
        return 0;
    }
    co->state = _CO_STATE_RUN;
    return co->func(co, co->data);
}

int co_stop(Co *co)
{
    co->state = _CO_STATE_FIN;
    return 0;
}

int co_done(Co *co)
{
    return co->state == _CO_STATE_FIN;
}

#endif // __TINY_CO_H__
