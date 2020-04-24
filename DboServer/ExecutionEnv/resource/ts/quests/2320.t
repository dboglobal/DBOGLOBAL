CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2320;
	title = 232002;

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
				conv = 232009;
				ctype = 1;
				idx = 1441101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 232008;
				gtype = 2;
				area = 232001;
				goal = 232004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 232005;
				stype = 1;
				taid = 1;
				title = 232002;
			}
			CDboTSActSToCEvt
			{
				widx1 = 1;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 232021;
				esctype = 0;
				iidx1 = -1;
				it1 = 232022;
				otype0 = 1;
				dt1 = 232023;
				etype = 0;
				it0 = 232020;
				taid = 3;
				tidx1 = 5721501;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 5721401;
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
			cont = 232008;
			gtype = 2;
			oklnk = 2;
			area = 232001;
			goal = 232004;
			sort = 232005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 232002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 232014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 232001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1441101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 232007;
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
				and = "2319;";
				flink = 1;
				flinknextqid = "2321;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1441101;";
			}
		}
	}
}

