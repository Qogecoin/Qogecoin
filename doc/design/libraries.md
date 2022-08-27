# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libqogecoin_cli*         | RPC client functionality used by *qogecoin-cli* executable |
| *libqogecoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libqogecoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libqogecoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libqogecoin_node* and *libqogecoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libqogecoinconsensus*    | Shared library build of static *libqogecoin_consensus* library |
| *libqogecoin_kernel*      | Consensus engine and support library used for validation by *libqogecoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libqogecoinqt*           | GUI functionality used by *qogecoin-qt* and *qogecoin-gui* executables |
| *libqogecoin_ipc*         | IPC functionality used by *qogecoin-node*, *qogecoin-wallet*, *qogecoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libqogecoin_node*        | P2P and RPC server functionality used by *qogecoind* and *qogecoin-qt* executables. |
| *libqogecoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libqogecoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libqogecoin_wallet*      | Wallet functionality used by *qogecoind* and *qogecoin-wallet* executables. |
| *libqogecoin_wallet_tool* | Lower-level wallet functionality used by *qogecoin-wallet* executable. |
| *libqogecoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *qogecoind* and *qogecoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libqogecoin_consensus* and *libqogecoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libqogecoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libqogecoin_node* code lives in `src/node/` in the `node::` namespace
  - *libqogecoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libqogecoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libqogecoin_util* code lives in `src/util/` in the `util::` namespace
  - *libqogecoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "linear" }}}%%

graph TD;

qogecoin-cli[qogecoin-cli]-->libqogecoin_cli;

qogecoind[qogecoind]-->libqogecoin_node;
qogecoind[qogecoind]-->libqogecoin_wallet;

qogecoin-qt[qogecoin-qt]-->libqogecoin_node;
qogecoin-qt[qogecoin-qt]-->libqogecoinqt;
qogecoin-qt[qogecoin-qt]-->libqogecoin_wallet;

qogecoin-wallet[qogecoin-wallet]-->libqogecoin_wallet;
qogecoin-wallet[qogecoin-wallet]-->libqogecoin_wallet_tool;

libqogecoin_cli-->libqogecoin_common;
libqogecoin_cli-->libqogecoin_util;

libqogecoin_common-->libqogecoin_util;
libqogecoin_common-->libqogecoin_consensus;

libqogecoin_kernel-->libqogecoin_consensus;
libqogecoin_kernel-->libqogecoin_util;

libqogecoin_node-->libqogecoin_common;
libqogecoin_node-->libqogecoin_consensus;
libqogecoin_node-->libqogecoin_kernel;
libqogecoin_node-->libqogecoin_util;

libqogecoinqt-->libqogecoin_common;
libqogecoinqt-->libqogecoin_util;

libqogecoin_wallet-->libqogecoin_common;
libqogecoin_wallet-->libqogecoin_util;

libqogecoin_wallet_tool-->libqogecoin_util;
libqogecoin_wallet_tool-->libqogecoin_wallet;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class qogecoin-qt,qogecoind,qogecoin-cli,qogecoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libqogecoin_wallet* and *libqogecoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code still is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libqogecoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libqogecoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libqogecoin_common* should serve a similar function as *libqogecoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libqogecoin_util* and *libqogecoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for qogecoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libqogecoin_kernel* is not supposed to depend on *libqogecoin_common*, only *libqogecoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libqogecoin_kernel* should only depend on *libqogecoin_util* and *libqogecoin_consensus*.

- The only thing that should depend on *libqogecoin_kernel* internally should be *libqogecoin_node*. GUI and wallet libraries *libqogecoinqt* and *libqogecoin_wallet* in particular should not depend on *libqogecoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libqogecoin_consensus*, *libqogecoin_common*, and *libqogecoin_util*, instead of *libqogecoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libqogecoinqt*, *libqogecoin_node*, *libqogecoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libqogecoin_node* to *libqogecoin_kernel* as part of [The libqogecoinkernel Project #24303](https://github.com/qogecoin/qogecoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/qogecoin/qogecoin/issues/15732)
