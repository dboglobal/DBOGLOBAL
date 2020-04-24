CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 118;
	title = 11802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
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
			cid = 103;
			elnk = 255;
			nextlnk = 254;
			prelnk = "102;";

			CDboTSActQItem
			{
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 102;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 103;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 0;
				iidx = -1;
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
			prelnk = "102;103;";
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
			stdiag = 11807;
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
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "5062103;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 8;
				eitype = 0;
				idx0 = 2131107;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1711103;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActNPCConv
			{
				conv = 11809;
				ctype = 1;
				idx = 5062103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 11808;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 11805;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "5679.620117";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 11801;
				goal = 11804;
				m0fz = "3052.810059";
				m0widx = 1;
				m1ttip = 11815;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "3026.199951";
				m3ttip = 11816;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "2836.189941";
				m2fx = "5888.009766";
				m2ttip = 11816;
				m0fx = "5663.379883";
				m0ttip = 11815;
				m3pat = 2;
				title = 11802;
				m1fx = "5896.370117";
				m2fz = "2798.989990";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 11814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 11801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 0;
				iidx = -1;
			}
			CDboTSClickNPC
			{
				npcidx = "5062103;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 11808;
			gtype = 2;
			oklnk = 2;
			area = 11801;
			goal = 11804;
			sort = 11805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 11802;
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

