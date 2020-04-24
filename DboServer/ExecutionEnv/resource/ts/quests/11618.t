CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11618;
	title = 1100634;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "51;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 254;
			prelnk = "50;50;";

			CDboTSActSendSvrEvt
			{
				id = 16620;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
			CDboTSActSendSvrEvt
			{
				id = 16590;
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
				npcidx = 2991101;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 51;
			cid = 50;
			dt = 0;
			lilnk = 255;
			oklnk = 51;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "26;";
			dg = 1100957;
			gt = 0;
			oi = 2991101;
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 26;
			prelnk = "6;6;5;4;3;2;1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100590;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				flink = 0;
				or = "11617;";
			}
			CDboTSClickNPC
			{
				npcidx = 2991101;
			}
		}
		CDboTSContReward
		{
			canclnk = 37;
			cid = 26;
			desc = 1100764;
			nextlnk = 50;
			rwdtbl = 1161801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "10;";
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
			cancellnk = 10;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100596;
			gt = 0;
			oi = 2991101;
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 5;
			dt = 0;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100595;
			gt = 0;
			oi = 2991101;
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 4;
			dt = 0;
			lilnk = 255;
			oklnk = 5;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100594;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 2;
			dt = 0;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100592;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100593;
			gt = 0;
			oi = 2991101;
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 1;
			dt = 0;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "37;0;";
			dg = 1100591;
			gt = 0;
			oi = 2991101;
		}
	}
}

