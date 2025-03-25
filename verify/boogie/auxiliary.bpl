/****************************************************************

        This file offers reusable code across architectures.

        It assumes the following be defined by each architecture / verification target:

            datatype Instruction
                instruction types

            datatype Ordering
                different ordering types, like acquire and release ordering or fences

            function is_sc(orders: [Ordering] bool) : bool
                for RCsc
            
            function ppo(i, j: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool): bool 
                defining whether i --ppo-> j with the ordering() and effects


            axiom sc_impl is ...;
            telling which SC implementation is used (TrailingFence, LeadingFence, RCsc)

****************************************************************/




// This builtin boogie function returns const array
function {:builtin "MapConst"} ConstArray<T,U>(U): [T]U;

type StateIndex = int;



datatype Effect {
    // read(a,v,vis) == read at a the value v. vis means whether this read is visible to barriers
    read(addr: int, value: int, visible: bool),
    write(addr: int, value: int)
}
var last_load, last_store: StateIndex;
var step: StateIndex;
var atomic: [StateIndex,StateIndex] bool;
var effects: [StateIndex][Effect] bool;
var ordering: [StateIndex][Ordering] bool;




// No implementation needed
// Using explicit implementations makes it run very slow and it gives no extra information
// The version without implementation creates a generic operation for each case
// NOTE: There is an issue on the boogie repo with regards to this 
function bit_and(x: int, y: int): int;
function bit_or(x: int, y: int): int;
function bit_xor(x: int, y: int): int;

function bit_not(x: int): int;

axiom (forall x: int :: bit_not(bit_not(x)) == x); // double negation on bits
axiom (forall x: int, y: int :: bit_and(x, y) == bit_and(y, x));
axiom (forall x: int, y: int :: bit_or(x, y) == bit_or(y, x));
axiom (forall x: int, y: int :: bit_xor(x, y) == bit_xor(y, x));

const max: [int, int] int;
axiom max == (lambda x, y: int ::
    if x > y then x else y
);

const add: [int, int] int;
axiom add == (lambda x, y: int :: x + y);

const sub: [int, int] int;
axiom sub == (lambda x, y: int :: x - y);

const and: [int, int] int;
axiom and == (lambda x, y: int :: bit_and(x, y));

const or: [int, int] int;
axiom or == (lambda x, y: int :: bit_or(x, y));

const xor: [int, int] int;
axiom xor == (lambda x, y: int :: bit_xor(x, y));

function i2b(i:int) returns (bool) { i != 0 }
function b2i(b:bool) returns (int) { if b then 1 else 0 }


type OrderRelation = [StateIndex, StateIndex, StateIndex, [StateIndex][Ordering] bool, [StateIndex][Effect] bool] bool;

const order_rlx: OrderRelation;
axiom order_rlx == (lambda i, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
    true
);

const order_acq: OrderRelation;
axiom order_acq == (lambda load, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
    (forall i: StateIndex ::
        (i >= exit) && (exists e: Effect :: effects[i][e]) ==> ppo(load, i, ordering, effects)
    )
);

const order_rel: OrderRelation;
axiom order_rel == (lambda store, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
    (forall i: StateIndex ::
        (i < entry) && (exists e: Effect :: effects[i][e]) ==> ppo(i, store, ordering, effects)
    )
);

datatype SCImplementation { LeadingFence(), TrailingFence(), RCsc() }
const sc_impl: SCImplementation;

const order_acq_sc: OrderRelation;
axiom order_acq_sc == (lambda load, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
    order_acq[load, entry, exit, ordering, effects] &&
    if sc_impl is LeadingFence then
        // ordered with all previous operations
        (forall i: StateIndex ::
            (i < entry) && (exists e: Effect :: effects[i][e]) ==> ppo(i, load, ordering, effects)
        )
    else if sc_impl is TrailingFence then
        true
    else if sc_impl is RCsc then
        // ordered with all previous SC operations
        is_sc(ordering[load]) &&
        (forall i: StateIndex ::
            (i < entry) && (exists e: Effect :: effects[i][e]) ==> is_sc(ordering[i]) ==> ppo(i, load, ordering, effects)
        )
    else false
);

const order_rel_sc: OrderRelation;
axiom order_rel_sc == (lambda store, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
    order_rel[store, entry, exit, ordering, effects] &&
    if sc_impl is LeadingFence then
        true
    else if sc_impl is TrailingFence then
        // ordered with all later operations
        (forall i: StateIndex ::
            (i >= exit) && (exists e: Effect :: effects[i][e]) ==> ppo(store, i, ordering, effects)
        )
    else if sc_impl is RCsc then
        // ordered with all later SC operations
        is_sc(ordering[store]) &&
        (forall i: StateIndex ::
            (i >= exit) && (exists e: Effect :: effects[i][e]) ==> is_sc(ordering[i]) ==> ppo(store, i, ordering, effects)
        )
    else false
);


const order_fence_acq: OrderRelation;
axiom order_fence_acq == (lambda fence, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
        (forall i, j: StateIndex ::
            (i < entry) && (j >= exit) && (exists e: Effect :: effects[i][e] && e is read && e->visible) && (exists e: Effect :: effects[j][e])
                ==> ppo(i, j, ordering, effects))
);

const order_fence_rel: OrderRelation;
axiom order_fence_rel == (lambda fence, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
        (forall i, j: StateIndex ::
            (i < entry) && (j >= exit) && (exists e: Effect :: effects[i][e]) && (exists e: Effect :: effects[j][e] && e is write)
                ==> ppo(i, j, ordering, effects))
);
const order_fence_sc: OrderRelation;
axiom order_fence_sc == (lambda fence, entry, exit: StateIndex, ordering: [StateIndex][Ordering] bool, effects: [StateIndex][Effect] bool ::
        (forall i, j: StateIndex ::
            (i < entry) && (j >= exit) && (exists e: Effect :: effects[i][e]) && (exists e: Effect :: effects[j][e])
                ==> ppo(i, j, ordering, effects))
);



function no_writes(from, to, write: StateIndex): bool {
    (write < from || to <= write)
}



type RMWOp = [int, int, int] int;

const cmpset, add_op, sub_op, set_op, min_op, max_op, ret_old: RMWOp;

axiom cmpset == (lambda x, y1, y2 : int :: if x == y1 then y2 else x);
axiom add_op == (lambda x, y, _: int :: x + y);
axiom sub_op == (lambda x, y, _: int :: x - y);
axiom set_op == (lambda x, y, _: int :: y);
axiom min_op == (lambda x, y, _: int :: if x < y then x else y);
axiom max_op == (lambda x, y, _: int :: if x < y then y else x);

axiom ret_old == (lambda x, _1, _2 : int :: x);

const bit8, bit16 : [RMWOp] RMWOp;
axiom bit8 == (lambda op : RMWOp :: (lambda x, y1, y2 : int :: op[x, bit_and(y1,255), bit_and(y2, 255)]));
axiom bit16 == (lambda op : RMWOp :: (lambda x, y1, y2 : int :: op[x, bit_and(y1,65535), bit_and(y2, 65535)]));

type AwaitOp = [int, int] bool;

const eq, neq, lt, le, gt, ge: AwaitOp;

axiom eq  == (lambda x, y: int :: x == y);
axiom neq == (lambda x, y: int :: x != y);
axiom lt  == (lambda x, y: int :: x <  y);
axiom le  == (lambda x, y: int :: x <= y);
axiom gt  == (lambda x, y: int :: x >  y);
axiom ge  == (lambda x, y: int :: x >= y);