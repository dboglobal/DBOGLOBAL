CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1535;
	title = 153502;

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
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 153507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "5061110;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 153514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 153501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061110;";
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
				conv = 153509;
				ctype = 1;
				idx = 5061110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 153508;
				gtype = 2;
				area = 153501;
				goal = 153504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 153505;
				stype = 2;
				taid = 1;
				title = 153502;
			}
			CDboTSActSToCEvt
			{
				dt2 = 153525;
				otype2 = 7;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 7;
				widx2 = 1;
				dt0 = 153521;
				esctype = 0;
				iidx1 = -1;
				it1 = 153522;
				otype0 = 7;
				tidx2 = 810;
				dt1 = 153523;
				etype = 0;
				iidx2 = -1;
				it0 = 153520;
				taid = 3;
				tidx1 = 809;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 808;
				ectype = -1;
				it2 = 153524;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 153508;
			gtype = 2;
			oklnk = 2;
			area = 153501;
			goal = 153504;
			sort = 153505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 153502;
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
	}
}

