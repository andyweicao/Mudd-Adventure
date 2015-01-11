//This module decode input data to color data, flag data, also decide the priority of the pictures.

//input data [21:0] 3D display ,[24 :22] : 000:enermy1 001: enermy2 010: box1 011:box2 1XX: NOTHING
module Data_Decoder (

			 input logic        clk,write,
	       input logic 	  reset, chipselect,
	       input logic [31:0]  writedata,
			 input logic [9:0]  vcount,
			 input logic [10:0] hcount,
			 input logic [9:0] address,
	       output logic [23:0] color1,
			 output logic 		 shoot,
			 output logic [6:0] hp,
			 output logic [5:0] am,
			 output logic [1:0] clip, bag,
			 output logic [1:0]  wound);

   logic [15:0] addr1, addr2, addr3, addr4, addr5, addr6, addr7, addr8, addr9, addr10,addr11;
   logic [23:0] q1;//boss
	logic [23:0] q2;//gun
	logic [23:0] q3;//fire
	logic [23:0] q4;//Winwin
	logic [23:0] q5;// texture1
	logic [23:0] q6;//sky
	logic [23:0] q7;
	logic [23:0] q8;
	logic [23:0] q9;
	logic [23:0] q10; //texture2
	logic [23:0] q11;
	
  // logic[479:0] inScreen;
  // logic[479:0] color;
//	logic [31:0]  screen[0:479];
	
	logic [2:0] control;
	logic [2:0]  pickup;
   logic [7:0] boss_hp;
   
	logic [1:0] hurt, boss_sec;
	logic empty, game_win, game_lose, treasure_clip, treasure_heal;

	logic  count;
	logic [10:0]  htemp;
	assign htemp = 2*hcount[10:1];
	
	
	
	     fight fignt_process (.*);
		
		  ROM1 BOSS1 (.clock(count), .q(q1),.address(addr1));//boss1
		  ROM2 GUNROM  (.clock(count), .q(q2),.address(addr2));//gun	 
		  ROM3 FIREROM (.clock(count), .q(q3),.address(addr3));//fire
		  ROM4 Winwin (.clock(count), .q(q4),.address(addr4));//Winwin
		  ROM5 Texture1(.clock(count), .q(q5),.address(addr5));//texture1//minion
		  ROM6 Sky   (.clock(count), .q(q6),.address(addr6));//Sky
		 // ROM7 FINDBOX  (.clock(count), .q(q7),.address(addr7));//FIND A BOX
		  ROM8 BOX1		(.clock(count), .q(q8),.address(addr8));// AID
		  ROM9 BOX2 	(.clock(count), .q(q9),.address(addr9));// BULLET BOX
		  ROM10 Texture2 	(.clock(count), .q(q10),.address(addr10));// texture2//brick
		  ROM11 GAMEOVER (.clock(count), .q(q11),.address(addr11));
		 


		
	
		
		//ASK A RAM TO STORE DATA
		logic [63:0] indata;
	
		logic inScreen,color,win;
	
		RAM1 register1 (.clock(clk),
	                .wraddress(address),.rdaddress((959-2*vcount)),
						 .data(writedata),.q(indata[63:32]),
						 .wren(write));
		RAM2 register2 (.clock(clk),
	                .wraddress(address),.rdaddress((958-2*vcount)),
						 .data(writedata),.q(indata[31:0]),
						 .wren(write));
						 
				 
						 
		always_ff @(posedge clk )begin
			if (reset) begin
			inScreen = 0;
			color = 0;
			end 
			else 
			begin
			//shoot <= indata [30];
			inScreen = ((hcount>=(2*(indata[9:0])))&&(hcount<=(2*(640-indata[9:0]))));
		   color = (indata[21:20]==2'b01);
			win = (indata[21:20]==2'b11);
			count <= count +1;
			end	
		end
		
			always_ff @(posedge clk) begin
		//		inScreen = ((hcount>=(2*(indata[9:0])))&&(hcount<=(2*(640-indata[9:0]))));
		//      color = (indata[20]==1);
			   shoot <= indata [30];
			   control <= indata[28:26];
		end
		
		
		
		logic fire,gun,boss1,boss2,box1,box2,bhp,mhp,aim;
		assign fire = ( htemp >1015 && htemp < 1115 && vcount > 100 &&vcount < 150 ) &&(!empty)&&(control==4) && count ;
		assign gun  =  htemp > 1043 && htemp < 1279 && vcount > 0 && vcount < 160 ;
		assign boss1 =  (htemp > 551 && htemp < 851 && vcount> 185  && vcount < 285) && (boss_sec==1) ;
		assign boss2 =  (htemp > 551 && htemp < 851 && vcount> 185  && vcount < 285) && (boss_sec==2) ;
		assign box1 =	 (	htemp > 609 && htemp <689 && vcount >200  && vcount < 252) && (treasure_heal==1);
		assign box2 =   (	htemp > 609 && htemp <689 && vcount >200  && vcount < 252) && (treasure_clip==1);
		assign bhp = ( htemp > 1 && htemp < 33 && vcount < boss_hp );
		assign mhp = (  vcount > (423-hp) && vcount < 423 && htemp > 1215 && htemp < 1245);
		assign  aim= ((htemp > (579 + shoot *10)  && vcount > 238 && htemp < (599 + shoot*10) && vcount <242 )||     //left
					 (htemp > (679- shoot*10) && vcount > 238 && htemp < (699-shoot*10) && vcount <242)||            // right  
					 (htemp > 635 && vcount > (210 +shoot *10) && htemp < 643 && vcount <( 220+ shoot * 10))||       //up  
					 (htemp > 635 && vcount > (260 - shoot *10) && htemp < 643 && vcount < (270 -shoot *10)));       //down
		
	//	assign bullet1=( htemp > 550 && htemp < 670 && vcount >195 && vcount <235 )&& (wound == 1);
	//	assign bullet2=( htemp > 580 && htemp < 700 && vcount >215 && vcount <255 )&& (wound == 2);
	//	assign bullet3=( htemp > 500 && htemp < 620 && vcount >245 && vcount <285 )&& (wound == 3);
		assign sky= (htemp < 641 );
		assign ground =( htemp > 639 && htemp < 1279);
//		assign inScreen = ((htemp>=(2*(screen[479-vcount][9:0])))&&(htemp<=(2*(640-screen[479-vcount][9:0]))));
//		assign color = (screen[479-vcount][20]==1);

	   logic [31:0]  temp,addr_wall;
		
		
		assign temp={16'h0,indata[61:46]};
		//assign addr_wall =(indata[45:32]+(((2*hcount[10:1]-{indata[9:0],1'b0})*temp)/16384));	
	  assign addr_wall =(indata[45:32]+(((htemp-{indata[9:0],1'b0})*temp)/16384));	
	  
		always_ff @(posedge count) begin
		if (sky) begin //sky
				//addr6 <= (((479-vcount)*128)/480+hcount/5);
			   addr6 <= (vcount *2+ htemp)/ 5;
				color1 <= q6;
		end
		
		if (ground) begin color1 <= {8'hd2, 8'hb4, 8'h84}; // ground
		end
		
	/*	if (inScreen && color) begin   //Mikilin
					addr10 <= (128*screen[479-vcount][16:10]+((htemp-2*screen[479-vcount][9:0])*64)/(640-2*screen[479-vcount][9:0]));	
					color1<=q10;
		end
		
		else if(|inScreen)begin //minions
  					addr5 <= (128*screen[479-vcount][16:10]+((htemp-2*screen[479-vcount][9:0])*64)/(640-2*screen[479-vcount][9:0]));
		         color1<=q5;
		end
	*/			
		
		
 		if (inScreen && color) begin   //Mikilin
					addr10 <= addr_wall;
				//	addr10 <= ({indata[16:10],7'b0}+({(htemp-{indata[9:0],1'b0}),6'b0})/(640-{indata[9:0],1'b0}));	
					//addr10 <= vcount;
					//addr10 <= (indata[45:32]+(((htemp-{indata[9:0],1'b0})*indata[63:46])/16384));
				
					color1<=q10;
		end
		
		else if (inScreen && win) begin   //Mikilin
					addr4 <= addr_wall;
				//	addr10 <= ({indata[16:10],7'b0}+({(htemp-{indata[9:0],1'b0}),6'b0})/(640-{indata[9:0],1'b0}));	
					//addr10 <= vcount;
					//addr10 <= (indata[45:32]+(((htemp-{indata[9:0],1'b0})*indata[63:46])/16384));
				
					color1<=q4;
		end
		
		else if(inScreen)begin //minions
  					addr5 <= addr_wall;
					//addr5 <= ({indata[16:10],7'b0}+({(htemp-{indata[9:0],1'b0}),6'b0})/(640-{indata[9:0],1'b0}));	
		         //addr5 <= (indata[45:32]+(((htemp-2*indata[9:0])*indata[63:46])/16384));
					color1<=q5; 
		end
		
	   if ( boss1) begin       //boss1
						addr1 <= (vcount-186)*300+(htemp-551);
						if (q1 != {8'hff,8'hff,8'hff})
						color1<=q1;	
		end
		
		if ( boss2 ) begin       //boss2
						addr1 <= (vcount-186)*300+(htemp-551);
						if (q1 != {8'hff,8'hff,8'hff})
						color1<=q1;	
		end
		
		if ( box1 ) begin       //box1
						addr8 <= (vcount-201)*78+(htemp-611);
						if (q8 != {8'hff,8'hff,8'hff})
						color1<=q8;	
		end
		
		if ( box2) begin       //box2
						addr9 <= (vcount-201)*78+(htemp-611);
						if (q9 != {8'hff,8'hff,8'hff})
						color1<=q9;	
		end
		
		if (fire) begin  //fire
				addr3 <= (vcount-101)*100+(htemp-1005);
					if ( q3 != {8'hff, 8'hff, 8'hff})
						color1 <= q3;
		end		 
		
		if ( gun ) begin    //gun
					addr2 <= (vcount)*240+htemp-1035;
						if (q2 != {8'hff,8'hff,8'hff})
						color1<=q2;
		end
		
	/*	if (bullet1) begin     //boom
							addr4 <= (vcount-195)*120+(htemp-550);	
							if (q4 != {8'hff,8'hff,8'hff})
							color1 <=q4;
		end
		if (bullet2) begin     //boom
							addr4 <= (vcount-215)*120+(htemp-580);	
							if (q4 != {8'hff,8'hff,8'hff})
							color1 <=q4;
		end
		if (bullet3) begin     //boom
							addr4 <= (vcount-245)*120+(htemp-500);	
							if (q4 != {8'hff,8'hff,8'hff})
							color1 <=q4;
		end	
	*/
		
		if (aim) begin
			color1<= {8'h00, 8'hff, 8'h00}; //green
		end
		if ( bhp) begin
							color1 <= {8'hb2, 8'h22, 8'h22};//red
		end
		if ( mhp ) begin //HP_sh
		               color1 <= {8'hb2, 8'h22, 8'h22}; // Red
		end

		if ( vcount > (423-60)  && vcount < 423 && htemp > 1250 && htemp < 1280 && (vcount%3!=0)) begin //POWER_sh
			color1<= {8'hdc, 8'hdc, 8'hdc}; // grey loss
		end
		
	   if ( vcount > (423-am)  && vcount < 423 && htemp > 1250 && htemp < 1280 && (vcount%3!=0))  begin //POWER_sh
			color1<= {8'h00, 8'h80, 8'h00}; // green
		end
		
		if (( vcount > 464 && vcount < 466 && htemp > 1216 && htemp < 1247)||( vcount > 464 && vcount < 466 && htemp > 1248 && htemp < 1277 ))  begin
		color1 <= {8'hA5, 8'h2A, 8'h2A}; 
		end
		if (game_lose) begin
				/*		color1 <= {8'h00,8'h00,8'h00};
						end
						else if ( (530 < htemp < 726) && (100 < vcount < 266)) begin*/
						addr11 <= (vcount)/3*65+(htemp/3);
						color1 <= q11;
						end
			
		 
		
		end	 
			endmodule
			