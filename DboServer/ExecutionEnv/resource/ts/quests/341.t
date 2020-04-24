CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 341;
	title = 34102;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 34109;
				ctype = 1;
				idx = 4751401;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 4;
				eitype = 1;
				idx0 = 500;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 501;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 34108;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 34105;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "5238.799805";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 34101;
				goal = 34104;
				m0fz = "-1719.780029";
				m0widx = 1;
				m1ttip = 34115;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-1533.800049";
				m3ttip = 34116;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1508.359985";
				m2fx = "4742.049805";
				m2ttip = 34116;
				m0fx = "4892.189941";
				m0ttip = 34115;
				m3pat = 2;
				title = 34102;
				m1fx = "5385.229980";
				m2fz = "-1776.489990";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 34108;
			gtype = 2;
			oklnk = 2;
			area = 34101;
			goal = 34104;
			sort = 34105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 34102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 34114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 34101;
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
				npcidx = "4751401;";
			}
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
			stdiag = 34107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "4751401;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 501;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 500;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
	}
}

