CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 989;
	title = 98902;

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
				conv = 98909;
				ctype = 1;
				idx = 5062104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 98908;
				gtype = 2;
				area = 98901;
				goal = 98904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 98905;
				stype = 1;
				taid = 1;
				title = 98902;
			}
			CDboTSActSToCEvt
			{
				dt2 = 98925;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 98921;
				esctype = 0;
				iidx1 = -1;
				it1 = 98922;
				otype0 = 1;
				tidx2 = 3172105;
				dt1 = 98923;
				etype = 0;
				iidx2 = -1;
				it0 = 98920;
				taid = 3;
				tidx1 = 3183202;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 1353104;
				ectype = -1;
				it2 = 98924;
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
			prelnk = "1;0;252;";
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
			cont = 98908;
			gtype = 2;
			oklnk = 2;
			area = 98901;
			goal = 98904;
			sort = 98905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 98902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 98914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 98901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5062104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 98907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 35;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "982;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5062104;";
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
}

