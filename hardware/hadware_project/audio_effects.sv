module audio_effects (
    input  clk,
    input  sample_end,
    input  sample_req,
    output [15:0] audio_output,
    input  [15:0] audio_input,
    input    control
);

reg [15:0] romdatafire,stepdatastep;
reg [12:0]  index1 = 13'd0;
reg [14:0]  index2 = 15'd0;
reg [15:0] last_sample;
reg [15:0] dat;

assign audio_output = dat;

//parameter SINE     = 0;
//parameter FEEDBACK = 1;

ROM12 SHOOT (.address(index1),.q(romdatafire),.clock(clk));
//ROM13 STEP (.address(index2),.q(romdatastep),.clock(clk));


always @(posedge clk) begin
    
        if (control==1)//[SINE])
            //dat <= last_sample;
				 begin
            dat <= romdatafire;
			   if (index1 == 13'd8063)
                index1 <= 13'd0;
            else
				index1 <= index1+1'b1;
        end 	      
				
				
			/*	begin
            dat <= romdatastep;
			   if (index2 == 15'd28829)
                index2 <= 15'd28829;
            else
				index2 <= index2+1'b1;
        end */	      

   /*    else  if (control[FEEDBACK]) 
		  begin
            dat <= romdatafire;
			   if (index1 == 13'd8063)
                index1 <= 13'd8063;
            else
				index1 <= index1+1'b1;
        end 	      

	*/	  else
			dat <= 16'd0000;
		  end


endmodule
