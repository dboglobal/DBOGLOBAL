CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2390;
	title = 239002;

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
				conv = 239009;
				ctype = 1;
				idx = 7092108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 239008;
				gtype = 2;
				area = 239001;
				goal = 239004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 239005;
				stype = 1;
				taid = 1;
				title = 239002;
			}
			CDboTSActSToCEvt
			{
				dt2 = 239025;
				otype2 = 1;
				widx1 = 14;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 14;
				dt0 = 239021;
				esctype = 0;
				iidx1 = -1;
				it1 = 239022;
				otype0 = 1;
				tidx2 = 4151112;
				dt1 = 239023;
				etype = 0;
				iidx2 = -1;
				it0 = 239020;
				taid = 3;
				tidx1 = 4752207;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 4192108;
				ectype = -1;
				it2 = 239024;
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
			desc = 239014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 239001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7092108;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 239008;
			gtype = 2;
			oklnk = 2;
			area = 239001;
			goal = 239004;
			sort = 239005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 239002;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 239007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "7092108;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2391;";
				not = 0;
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
	}
}

