module digital_decode(

input   clk,reset,
input   logic [4:0] digital,
output  logic [7:0] digital_code
);

always_ff @ (posedge clk) begin
if (reset)
   digital_code = 8'b00111111; // 0
else begin
     case (digital) 
	0:digital_code <= 8'b00111111; // 0
	1:digital_code <= 8'b00000110; // 1
	2:digital_code <= 8'b01011011; // 2
	3:digital_code <= 8'b01001111; // 3
	4:digital_code <= 8'b01100110; // 4
	5:digital_code <= 8'b01101101; // 5
	6:digital_code <= 8'b01111101; // 6
	7:digital_code <= 8'b00000111; // 7
	8:digital_code <= 8'b01111111; // 8
	9:digital_code <= 8'b01101111; // 9
       endcase
end
end	
endmodule 
