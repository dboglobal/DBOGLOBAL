CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2355;
	title = 235502;

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
				conv = 235509;
				ctype = 1;
				idx = 5063109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 235508;
				gtype = 2;
				area = 235501;
				goal = 235504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 235505;
				stype = 1;
				taid = 1;
				title = 235502;
			}
			CDboTSActSToCEvt
			{
				dt2 = 235525;
				otype2 = 7;
				widx1 = 14;
				iidx0 = 1332;
				otype1 = 7;
				widx2 = 14;
				dt0 = 235521;
				esctype = 0;
				iidx1 = 1332;
				it1 = 235522;
				otype0 = 7;
				tidx2 = 18;
				dt1 = 235523;
				etype = 0;
				iidx2 = 1332;
				it0 = 235520;
				taid = 3;
				tidx1 = 17;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 16;
				ectype = -1;
				it2 = 235524;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 235514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 235501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063109;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 235508;
			gtype = 2;
			oklnk = 2;
			area = 235501;
			goal = 235504;
			sort = 235505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 235502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 235507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSCheckClrQst
			{
				and = "2354;";
				flink = 1;
				flinknextqid = "2356;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5063109;";
			}
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
				iidx0 = 1332;
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
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

