CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1641;
	title = 164102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 164125;
				otype2 = 1;
				widx1 = 1;
				iidx0 = 783;
				otype1 = 1;
				widx2 = 1;
				dt0 = 164121;
				esctype = 0;
				iidx1 = -1;
				it1 = 164122;
				otype0 = 1;
				tidx2 = 3242104;
				dt1 = 164123;
				etype = 0;
				iidx2 = -1;
				it0 = 164120;
				taid = 1;
				tidx1 = 7134102;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 7134104;
				ectype = -1;
				it2 = 164124;
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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 164109;
				ctype = 1;
				idx = 3142214;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 164108;
				gtype = 2;
				area = 164101;
				goal = 164104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 164105;
				stype = 1;
				taid = 1;
				title = 164102;
			}
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
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 164108;
			gtype = 2;
			oklnk = 2;
			area = 164101;
			goal = 164104;
			sort = 164105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 164102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 164114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 164101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142214;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 164107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSCheckClrQst
			{
				and = "1640;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3142214;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
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

