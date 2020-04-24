CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2298;
	title = 229802;

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
				conv = 229809;
				ctype = 1;
				idx = 3181305;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 6311206;
				ectype = -1;
				etype = 0;
				iidx0 = 1409;
				it0 = 229820;
				taid = 3;
				widx0 = 1;
				dt0 = 229821;
				esctype = 0;
				otype0 = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 229808;
				gtype = 2;
				area = 229801;
				goal = 229804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 229805;
				stype = 1;
				taid = 1;
				title = 229802;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 229808;
			gtype = 2;
			oklnk = 2;
			area = 229801;
			goal = 229804;
			sort = 229805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 229802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 229807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				and = "2297;";
				flink = 1;
				flinknextqid = "2299;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3181305;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 229814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 229801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3181305;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1409;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 229807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

