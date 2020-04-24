CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2650;
	title = 265002;

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
				conv = 265009;
				ctype = 1;
				idx = 4192106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 265008;
				gtype = 2;
				area = 265001;
				goal = 265004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 265005;
				stype = 1;
				taid = 1;
				title = 265002;
			}
			CDboTSActSToCEvt
			{
				dt2 = 265025;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 265021;
				esctype = 0;
				iidx1 = -1;
				it1 = 265022;
				otype0 = 1;
				tidx2 = 2951109;
				dt1 = 265023;
				etype = 0;
				iidx2 = -1;
				it0 = 265020;
				taid = 3;
				tidx1 = 1351124;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 5591208;
				ectype = -1;
				it2 = 265024;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 265008;
			gtype = 2;
			oklnk = 2;
			area = 265001;
			goal = 265004;
			sort = 265005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 265002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 265014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 265001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4192106;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 265007;
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
				npcidx = "4192106;";
			}
			CDboTSCheckClrQst
			{
				and = "2649;";
				flink = 1;
				flinknextqid = "2651;";
				not = 0;
			}
		}
	}
}

