CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 191;
	title = 19102;

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
				eitype = 1;
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
			stdiag = 19107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "5591201;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 19109;
				ctype = 1;
				idx = 5591201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1018;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 19108;
				m1fx = "-51.000000";
				m2fz = "4733.000000";
				m1fy = "-50.000000";
				sort = 19105;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 2;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 19101;
				goal = 19104;
				m0fz = "-893.909973";
				m0widx = 2;
				m1ttip = 19116;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 16;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1039.000000";
				m2fx = "4236.000000";
				m2ttip = 19115;
				m0fx = "757.179993";
				m0ttip = 19116;
				title = 19102;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 19108;
			gtype = 3;
			oklnk = 2;
			area = 19101;
			goal = 19104;
			sort = 19105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 19102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 19114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 19101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "5591201;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 19107;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1018;
				taid = 1;
				type = 1;
			}
		}
	}
}

