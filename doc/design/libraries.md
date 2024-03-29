# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libvaldcoin_cli*         | RPC client functionality used by *valdcoin-cli* executable |
| *libvaldcoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libvaldcoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libvaldcoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libvaldcoin_node* and *libvaldcoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libvaldcoinconsensus*    | Shared library build of static *libvaldcoin_consensus* library |
| *libvaldcoin_kernel*      | Consensus engine and support library used for validation by *libvaldcoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libvaldcoinqt*           | GUI functionality used by *valdcoin-qt* and *valdcoin-gui* executables |
| *libvaldcoin_ipc*         | IPC functionality used by *valdcoin-node*, *valdcoin-wallet*, *valdcoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libvaldcoin_node*        | P2P and RPC server functionality used by *valdcoind* and *valdcoin-qt* executables. |
| *libvaldcoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libvaldcoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libvaldcoin_wallet*      | Wallet functionality used by *valdcoind* and *valdcoin-wallet* executables. |
| *libvaldcoin_wallet_tool* | Lower-level wallet functionality used by *valdcoin-wallet* executable. |
| *libvaldcoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *valdcoind* and *valdcoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libvaldcoin_consensus* and *libvaldcoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libvaldcoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libvaldcoin_node* code lives in `src/node/` in the `node::` namespace
  - *libvaldcoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libvaldcoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libvaldcoin_util* code lives in `src/util/` in the `util::` namespace
  - *libvaldcoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

valdcoin-cli[valdcoin-cli]-->libvaldcoin_cli;

valdcoind[valdcoind]-->libvaldcoin_node;
valdcoind[valdcoind]-->libvaldcoin_wallet;

valdcoin-qt[valdcoin-qt]-->libvaldcoin_node;
valdcoin-qt[valdcoin-qt]-->libvaldcoinqt;
valdcoin-qt[valdcoin-qt]-->libvaldcoin_wallet;

valdcoin-wallet[valdcoin-wallet]-->libvaldcoin_wallet;
valdcoin-wallet[valdcoin-wallet]-->libvaldcoin_wallet_tool;

libvaldcoin_cli-->libvaldcoin_util;
libvaldcoin_cli-->libvaldcoin_common;

libvaldcoin_common-->libvaldcoin_consensus;
libvaldcoin_common-->libvaldcoin_util;

libvaldcoin_kernel-->libvaldcoin_consensus;
libvaldcoin_kernel-->libvaldcoin_util;

libvaldcoin_node-->libvaldcoin_consensus;
libvaldcoin_node-->libvaldcoin_kernel;
libvaldcoin_node-->libvaldcoin_common;
libvaldcoin_node-->libvaldcoin_util;

libvaldcoinqt-->libvaldcoin_common;
libvaldcoinqt-->libvaldcoin_util;

libvaldcoin_wallet-->libvaldcoin_common;
libvaldcoin_wallet-->libvaldcoin_util;

libvaldcoin_wallet_tool-->libvaldcoin_wallet;
libvaldcoin_wallet_tool-->libvaldcoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class valdcoin-qt,valdcoind,valdcoin-cli,valdcoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libvaldcoin_wallet* and *libvaldcoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libvaldcoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libvaldcoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libvaldcoin_common* should serve a similar function as *libvaldcoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libvaldcoin_util* and *libvaldcoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for valdcoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libvaldcoin_kernel* is not supposed to depend on *libvaldcoin_common*, only *libvaldcoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libvaldcoin_kernel* should only depend on *libvaldcoin_util* and *libvaldcoin_consensus*.

- The only thing that should depend on *libvaldcoin_kernel* internally should be *libvaldcoin_node*. GUI and wallet libraries *libvaldcoinqt* and *libvaldcoin_wallet* in particular should not depend on *libvaldcoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libvaldcoin_consensus*, *libvaldcoin_common*, and *libvaldcoin_util*, instead of *libvaldcoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libvaldcoinqt*, *libvaldcoin_node*, *libvaldcoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libvaldcoin_node* to *libvaldcoin_kernel* as part of [The libvaldcoinkernel Project #24303](https://github.com/valdcoin/valdcoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/valdcoin/valdcoin/issues/15732)
