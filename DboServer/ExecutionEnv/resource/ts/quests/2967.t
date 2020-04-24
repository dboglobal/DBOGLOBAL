CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2967;
	title = 296702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99154;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 296714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 296701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751210;";
			}
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
			stdiag = 296707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4751210;";
			}
			CDboTSCheckClrQst
			{
				and = "2966;";
				flink = 0;
				not = 0;
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
				conv = 296709;
				ctype = 1;
				idx = 4751210;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99154;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 2967;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 296715;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 296708;
				area = 296701;
				goal = 296704;
				m0fz = "-4530.689941";
				m0widx = 1;
				scitem = 99154;
				sort = 296705;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = -1;
				m0fx = "4402.160156";
				m0ttip = 296715;
				rwd = 100;
				taid = 1;
				title = 296702;
				gtype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
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
				itype = 5;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 296708;
			gtype = 1;
			oklnk = 2;
			area = 296701;
			goal = 296704;
			sort = 296705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 296702;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99154;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
	}
}

