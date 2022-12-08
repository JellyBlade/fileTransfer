Tyler Hippard, Landon Constantin

To compile:
> make

Run sender:
> ./fileTransfer_sender destination_host destination_port

Run receiver:
> ./fileTransfer_receiver receive_port

Send from a file:
> ./fileTransfer_sender -f [filename] destination_host destination_port

Receive and write to a file:
> ./fileTransfer_receiver -f [filename] receive_port

For documentation(doxygen):
> make docs

For testing(GTEST, valgrind, lcov):
> make allTests
