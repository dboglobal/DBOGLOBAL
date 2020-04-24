CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1668;
	title = 166802;

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
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 166807;
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
				npcidx = "7143102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 166808;
			gtype = 2;
			oklnk = 2;
			area = 166801;
			goal = 166804;
			sort = 166805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 166802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 166814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 166801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7143102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 166825;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 166821;
				esctype = 0;
				iidx1 = -1;
				it1 = 166822;
				otype0 = 1;
				tidx2 = 7091105;
				dt1 = 166823;
				etype = 0;
				iidx2 = -1;
				it0 = 166820;
				taid = 1;
				tidx1 = 2611204;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 7134105;
				ectype = -1;
				it2 = 166824;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
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
				conv = 166809;
				ctype = 1;
				idx = 7143102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 166808;
				gtype = 2;
				area = 166801;
				goal = 166804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 166805;
				stype = 1;
				taid = 1;
				title = 166802;
			}
		}
	}
}

