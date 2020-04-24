CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2455;
	title = 245502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 245509;
				ctype = 1;
				idx = 4141116;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 245508;
				gtype = 2;
				area = 245501;
				goal = 245504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 245505;
				stype = 1;
				taid = 1;
				title = 245502;
			}
			CDboTSActSToCEvt
			{
				dt2 = 245525;
				otype2 = 7;
				widx1 = 14;
				iidx0 = -1;
				otype1 = 7;
				widx2 = 14;
				dt0 = 245521;
				esctype = 0;
				iidx1 = -1;
				it1 = 245522;
				otype0 = 7;
				tidx2 = 139;
				dt1 = 245523;
				etype = 0;
				iidx2 = -1;
				it0 = 245520;
				taid = 3;
				tidx1 = 138;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 137;
				ectype = -1;
				it2 = 245524;
			}
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
				itype = 6;
			}
		}
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 245514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 245501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141116;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 245508;
			gtype = 2;
			oklnk = 2;
			area = 245501;
			goal = 245504;
			sort = 245505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 245502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 245507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "4141116;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2456;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
}

