The sliderbar.proto contains the description of the protocol.
To transform it to a .h/.c files, use the command:

protoc --nanopb_out=. sliderbar.proto

in a terminal.
It should output two files called sliderbar.pb.