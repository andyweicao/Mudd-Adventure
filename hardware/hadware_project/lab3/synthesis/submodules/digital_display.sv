module digital_display(

input logic clk,reset,
input  logic [7:0] number,
output logic [7:0] tens_code,
output logic [7:0] nums_code
);

logic [4:0] tens;
logic [4:0] nums;

assign tens=number/10;
assign nums=number%10;

digital_decode decode_tens (.digital(tens),.digital_code(tens_code),.*);
digital_decode decode_nums (.digital(nums),.digital_code(nums_code),.*);

endmodule 