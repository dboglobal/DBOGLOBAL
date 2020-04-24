CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11627;
	title = 1100733;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "38;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 38;
			elnk = 255;
			nextlnk = 254;
			prelnk = "26;";

			CDboTSActSendSvrEvt
			{
				id = 17040;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
			CDboTSActSendSvrEvt
			{
				id = 16950;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 37;
			prelnk = "26;";
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 1701101;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 26;
			prelnk = "8;8;7;6;5;4;3;2;1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "7;";
			dg = 1100741;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContReward
		{
			canclnk = 37;
			cid = 26;
			desc = 1100765;
			nextlnk = 38;
			rwdtbl = 1162701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "9;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "2;";
			dg = 1100736;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100740;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "5;";
			dg = 1100739;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100738;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "3;";
			dg = 1100737;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100732;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 1701101;
			}
			CDboTSCheckClrQst
			{
				and = "11626;";
				flink = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100735;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "37;0;";
			dg = 1100734;
			gt = 0;
			oi = 1701101;
		}
	}
}

