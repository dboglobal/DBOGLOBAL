CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11312;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 5;
			prelnk = "100;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 100;
			elnk = 255;
			nextlnk = 101;
			prelnk = "5;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1101056;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92003;
				eitype = 5;
				idx0 = 11513;
				cnt0 = 0;
				ectype = 0;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 13232;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 100;
			nextlnk = 6;
			prelnk = "101;4;";

			CDboTSActRegQInfo
			{
				cont = 1100463;
				gtype = 2;
				area = 1100262;
				goal = 1100462;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100461;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92003;
				eitype = 5;
				idx0 = 11513;
				cnt0 = 1;
				ectype = 0;
				etype = 0;
				qtidx = 1100273;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 13220;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100235;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100236;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;1;2;";

			CDboTSActSendSvrEvt
			{
				id = 13230;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 13231;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
	}
}

