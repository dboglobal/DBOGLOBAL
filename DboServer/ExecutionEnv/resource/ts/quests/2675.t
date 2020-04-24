CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2675;
	title = 267502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 267509;
				ctype = 1;
				idx = 1502201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 267508;
				gtype = 2;
				area = 267501;
				goal = 267504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 267505;
				stype = 2;
				taid = 1;
				title = 267502;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 2793101;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 267520;
				taid = 3;
				widx0 = 1;
				dt0 = 267521;
				esctype = 0;
				otype0 = 1;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 267507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "1502201;";
			}
			CDboTSCheckClrQst
			{
				and = "2674;";
				flink = 1;
				flinknextqid = "2677;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 267508;
			gtype = 2;
			oklnk = 2;
			area = 267501;
			goal = 267504;
			sort = 267505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 267502;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 267514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 267501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1502201;";
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

