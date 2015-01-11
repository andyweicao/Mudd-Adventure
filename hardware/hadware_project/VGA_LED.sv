 /*
 * Avalon memory-mapped peripheral for the VGA LED Emulator
 *
 * 
 * 
 */
 
// now this module is only about sub-module connection and digital tranfer

module VGA_LED(
			 input logic        clk,
	       input logic 	  reset,
	       input logic [31:0]  writedata,//control: writedata[7:0]
	       input logic 	  write,
	       input 		     chipselect,
	       input logic [9:0]  address,
	       output logic [7:0] VGA_R, VGA_G, VGA_B,
	       output logic 	  VGA_CLK, VGA_HS, VGA_VS, VGA_BLANK_n,
	       output logic 	  VGA_SYNC_n,
			 output logic    VGA_shoot);

	 
  
   logic [9:0]  vcount;
   logic [10:0] hcount;
	logic [23:0] color1;
//	logic 		 shoot;
	logic [1:0]  wound;
	 
	     VGA_LED_Emulator led_emulator(.clk50(clk), .*);
		  Data_Decoder data_decoder( .shoot(VGA_shoot),.*); 
		  
 logic [6:0] hp;
 logic [5:0] am;
 logic [1:0] clip, bag; 
 logic [7:0] hp_t,hp_n,am_t,am_n,clip_t,clip_n,bag_t,bag_n,hex90_t,hex90_n,hex60_t,hex60_n;

		  digital_display digital_am(.number(am),.tens_code(am_t),.nums_code(am_n),.*);		  
		  digital_display digital_hp(.number(hp),.tens_code(hp_t),.nums_code(hp_n),.*);	
		  digital_display digital_clip(.number(clip),.tens_code(clip_t),.nums_code(clip_n),.*);		
		  digital_display digital_bag(.number(bag),.tens_code(bag_t),.nums_code(bag_n ),.*);		 
  
  digital_display digital_90(.number({8'd90}),.tens_code(hex90_t),.nums_code(hex90_n),.*);
  digital_display digital_60(.number({8'd60}),.tens_code(hex60_t),.nums_code(hex60_n),.*);
endmodule
			 
			 
			 
