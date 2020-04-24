CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11617;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 37;
			elnk = 255;
			nextlnk = 3;
			prelnk = "36;";

			CDboTSActSendSvrEvt
			{
				id = 16520;
				tblidx = 1711101;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 36;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 37;

			CDboTSClickNPC
			{
				npcidx = 1711201;
			}
		}
		CDboTSContGAct
		{
			cid = 35;
			elnk = 255;
			nextlnk = 254;
			prelnk = "34;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1100637;
			}
		}
		CDboTSContGCond
		{
			cid = 34;
			prelnk = "31;";
			nolnk = 255;
			rm = 0;
			yeslnk = 35;

			CDboTSRcvSvrEvt
			{
				id = 16610;
			}
		}
		CDboTSContGCond
		{
			cid = 33;
			prelnk = "32;";
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 16515;
			}
		}
		CDboTSContGAct
		{
			cid = 32;
			elnk = 255;
			nextlnk = 33;
			prelnk = "0;";

			CDboTSActRegQInfo
			{
				cont = 1100633;
				gtype = -1;
				area = 1100636;
				goal = 1100632;
				grade = -1;
				rwd = 255;
				sort = 1100406;
				stype = 32;
				taid = 1;
				title = 1100631;
			}
		}
		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 17;
			prelnk = "9;10;11;12;13;14;15;15;";

			CDboTSActSendSvrEvt
			{
				id = 16560;
				tblidx = 11605;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 30;
			dt = 0;
			lilnk = 255;
			oklnk = 31;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "29;";
			dg = 1100589;
			gt = 0;
			oi = 2741101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 29;
			dt = 0;
			lilnk = 255;
			oklnk = 30;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "28;";
			dg = 1100588;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 28;
			dt = 0;
			lilnk = 255;
			oklnk = 29;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "27;";
			dg = 1100587;
			gt = 0;
			oi = 2741101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 27;
			dt = 0;
			lilnk = 255;
			oklnk = 28;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "26;";
			dg = 1100586;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 26;
			dt = 0;
			lilnk = 255;
			oklnk = 27;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "23;";
			dg = 1100585;
			gt = 0;
			oi = 2741101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 23;
			dt = 0;
			lilnk = 255;
			oklnk = 26;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "22;";
			dg = 1100583;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 22;
			dt = 0;
			lilnk = 255;
			oklnk = 23;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "21;";
			dg = 1100581;
			gt = 0;
			oi = 2741101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 21;
			dt = 0;
			lilnk = 255;
			oklnk = 22;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "20;";
			dg = 1100580;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 20;
			dt = 0;
			lilnk = 255;
			oklnk = 21;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "19;";
			dg = 1100579;
			gt = 0;
			oi = 2741101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 19;
			dt = 0;
			lilnk = 255;
			oklnk = 20;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "18;";
			dg = 1100578;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 18;
			dt = 0;
			lilnk = 255;
			oklnk = 19;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "17;";
			dg = 1100577;
			gt = 0;
			oi = 2741101;
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "3;4;5;6;6;";

			CDboTSActSendSvrEvt
			{
				id = 16540;
				tblidx = 11605;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 36;

			CDboTSRcvSvrEvt
			{
				id = 16530;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 15;
			dt = 0;
			lilnk = 255;
			oklnk = 16;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 3;
			prelnk = "14;";
			dg = 1100576;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContGCond
		{
			cid = 8;
			prelnk = "7;";
			nolnk = 255;
			rm = 0;
			yeslnk = 9;

			CDboTSRcvSvrEvt
			{
				id = 16550;
			}
		}
		CDboTSContGAct
		{
			cid = 31;
			elnk = 255;
			nextlnk = 34;
			prelnk = "30;30;29;28;27;26;23;22;21;20;19;18;";

			CDboTSActSendSvrEvt
			{
				id = 16580;
				tblidx = 11605;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 16600;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "35;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 17;
			prelnk = "16;";
			nolnk = 255;
			rm = 0;
			yeslnk = 18;

			CDboTSRcvSvrEvt
			{
				id = 16570;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "33;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1100565;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 14;
			dt = 0;
			lilnk = 255;
			oklnk = 15;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "13;";
			dg = 1100575;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 13;
			dt = 0;
			lilnk = 255;
			oklnk = 14;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 3;
			prelnk = "12;";
			dg = 1100574;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 12;
			dt = 0;
			lilnk = 255;
			oklnk = 13;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "11;";
			dg = 1100573;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 11;
			dt = 0;
			lilnk = 255;
			oklnk = 12;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1100572;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 10;
			dt = 0;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1100571;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 9;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1100570;
			gt = 0;
			oi = -1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 32;

			CDboTSRcvSvrEvt
			{
				id = 16514;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100569;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 5;
			dt = 0;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "4;";
			dg = 1100568;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 4;
			dt = 0;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100567;
			gt = 0;
			oi = 1711101;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "37;";
			dg = 1100566;
			gt = 0;
			oi = -1;
		}
	}
}

