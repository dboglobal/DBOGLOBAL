CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 920;
	title = 92002;

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
				conv = 92009;
				ctype = 1;
				idx = 6311204;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 92008;
				gtype = 2;
				area = 92001;
				goal = 92004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 92005;
				stype = 1;
				taid = 1;
				title = 92002;
			}
			CDboTSActSToCEvt
			{
				widx1 = 1;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 92021;
				esctype = 0;
				iidx1 = -1;
				it1 = 92022;
				otype0 = 1;
				dt1 = 92023;
				etype = 0;
				it0 = 92020;
				taid = 3;
				tidx1 = 3142110;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3142101;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 92008;
			gtype = 2;
			oklnk = 2;
			area = 92001;
			goal = 92004;
			sort = 92005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 92002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 92014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 92001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6311204;";
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 92007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
			CDboTSClickNPC
			{
				npcidx = "6311204;";
			}
			CDboTSCheckClrQst
			{
				and = "907;";
				flink = 1;
				not = 0;
			}
		}
	}
}

