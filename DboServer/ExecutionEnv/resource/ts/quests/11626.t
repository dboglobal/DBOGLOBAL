CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11626;
	title = 1100664;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 39;
			prelnk = "38;";
			nolnk = 255;
			rm = 0;
			yeslnk = 24;

			CDboTSRcvSvrEvt
			{
				id = 16886;
			}
		}
		CDboTSContGAct
		{
			cid = 38;
			elnk = 255;
			nextlnk = 39;
			prelnk = "18;";

			CDboTSActSendSvrEvt
			{
				id = 16885;
				tblidx = 11606;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGCond
		{
			cid = 37;
			prelnk = "36;";
			nolnk = 255;
			rm = 0;
			yeslnk = 9;

			CDboTSRcvSvrEvt
			{
				id = 16856;
			}
		}
		CDboTSContGAct
		{
			cid = 36;
			elnk = 255;
			nextlnk = 37;
			prelnk = "6;5;4;3;2;1;8;8;";

			CDboTSActSendSvrEvt
			{
				id = 16855;
				tblidx = 11606;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 35;
			elnk = 255;
			nextlnk = 254;
			prelnk = "34;34;33;32;";

			CDboTSActSendSvrEvt
			{
				id = 16920;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 31;
			elnk = 255;
			nextlnk = 32;
			prelnk = "26;27;28;29;30;30;";

			CDboTSActSendSvrEvt
			{
				id = 16910;
				tblidx = 1701101;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 25;
			prelnk = "24;";
			nolnk = 255;
			rm = 0;
			yeslnk = 26;

			CDboTSRcvSvrEvt
			{
				id = 16900;
			}
		}
		CDboTSContGAct
		{
			cid = 24;
			elnk = 255;
			nextlnk = 25;
			prelnk = "39;";

			CDboTSActSendSvrEvt
			{
				id = 16890;
				tblidx = 1701101;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 18;
			prelnk = "17;";
			nolnk = 255;
			rm = 0;
			yeslnk = 38;

			CDboTSCheckInNPC
			{
				idx = 1701101;
				rad = 10;
			}
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
				id = 16880;
			}
		}
		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 17;
			prelnk = "15;15;";

			CDboTSActSendSvrEvt
			{
				id = 16860;
				tblidx = 11606;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 15;
			prelnk = "13;";

			CDboTSActSendSvrEvt
			{
				id = 16980;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 13;
			prelnk = "12;";
			nolnk = 255;
			rm = 0;
			yeslnk = 14;

			CDboTSRcvSvrEvt
			{
				id = 16990;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 35;
			cid = 34;
			dt = 2;
			lilnk = 255;
			oklnk = 35;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "33;";
			dg = 1100728;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContNarration
		{
			cancellnk = 35;
			cid = 33;
			dt = 2;
			lilnk = 255;
			oklnk = 34;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 2;
			prelnk = "32;";
			dg = 1100727;
			gt = 0;
			oi = 3511104;
		}
		CDboTSContNarration
		{
			cancellnk = 35;
			cid = 32;
			dt = 2;
			lilnk = 255;
			oklnk = 33;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "31;";
			dg = 1100726;
			gt = 0;
			oi = 9042101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 30;
			dt = 2;
			lilnk = 255;
			oklnk = 31;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "29;";
			dg = 1100725;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 29;
			dt = 2;
			lilnk = 255;
			oklnk = 30;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "28;";
			dg = 1100724;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 28;
			dt = 2;
			lilnk = 255;
			oklnk = 29;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "27;";
			dg = 1100723;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 27;
			dt = 2;
			lilnk = 255;
			oklnk = 28;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "26;";
			dg = 1100722;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 31;
			cid = 26;
			dt = 2;
			lilnk = 255;
			oklnk = 27;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "25;";
			dg = 1100721;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "11;";

			CDboTSActSendSvrEvt
			{
				id = 16970;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "10;10;";

			CDboTSActSendSvrEvt
			{
				id = 16860;
				tblidx = 11606;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 15;
			dt = 2;
			lilnk = 255;
			oklnk = 16;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "14;";
			dg = 1100715;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 1;
			prelnk = "0;";

			CDboTSActSendSvrEvt
			{
				id = 16850;
				tblidx = 11606;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "35;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "9;9;";
			dg = 1100711;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 9;
			dt = 2;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "37;";
			dg = 1100710;
			gt = 0;
			oi = 1331101;
		}
		CDboTSContNarration
		{
			cancellnk = 36;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 36;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100709;
			gt = 0;
			oi = 3251101;
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
			stdiag = 1100702;
			nolnk = 253;
			rm = 0;
			yeslnk = 7;

			CDboTSClickNPC
			{
				npcidx = 1701101;
			}
			CDboTSCheckClrQst
			{
				and = "11625;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 36;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100708;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 36;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "4;";
			dg = 1100707;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 36;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100706;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 36;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "2;";
			dg = 1100705;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 36;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "7;";
			dg = 1100703;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 36;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100704;
			gt = 0;
			oi = -1;
		}
	}
}

