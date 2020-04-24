CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2493;
	title = 249302;

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
				conv = 249309;
				ctype = 1;
				idx = 4151212;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 249308;
				gtype = 2;
				area = 249301;
				goal = 249304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 249305;
				stype = 1;
				taid = 1;
				title = 249302;
			}
			CDboTSActSToCEvt
			{
				dt2 = 249325;
				otype2 = 1;
				widx1 = 14;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 14;
				dt0 = 249321;
				esctype = 0;
				iidx1 = -1;
				it1 = 249322;
				otype0 = 1;
				tidx2 = 1593101;
				dt1 = 249323;
				etype = 0;
				iidx2 = -1;
				it0 = 249320;
				taid = 3;
				tidx1 = 6314103;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 4951101;
				ectype = -1;
				it2 = 249324;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 249314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 249301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151212;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 249308;
			gtype = 2;
			oklnk = 2;
			area = 249301;
			goal = 249304;
			sort = 249305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 249302;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 249307;
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
				npcidx = "4151212;";
			}
			CDboTSCheckClrQst
			{
				and = "2491;";
				flink = 1;
				flinknextqid = "2494;";
				not = 0;
			}
		}
	}
}

