CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1765;
	title = 176502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 176507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "6111201;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 176509;
				ctype = 1;
				idx = 6111201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 0;
				idx0 = 4652300;
				cnt0 = 6;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 4261202;
				taid = 3;
				esctype = 0;
				idx2 = 4262300;
			}
			CDboTSActRegQInfo
			{
				cont = 176508;
				m1fx = "-40.000000";
				m2fz = "579.000000";
				m1fy = "0.000000";
				sort = 176505;
				m2widx = 10;
				grade = 132203;
				m1pat = 2;
				m1widx = 10;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 176501;
				goal = 176504;
				m0fz = "451.000000";
				m0widx = 10;
				m1ttip = 176516;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "311.000000";
				m2fx = "108.000000";
				m2ttip = 176517;
				m0fx = "-63.000000";
				m0ttip = 176515;
				title = 176502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 176514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 176501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 176508;
			gtype = 3;
			oklnk = 2;
			area = 176501;
			goal = 176504;
			sort = 176505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 176502;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

