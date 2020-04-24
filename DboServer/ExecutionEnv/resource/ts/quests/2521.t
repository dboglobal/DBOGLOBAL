CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2521;
	title = 252102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 99103;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActItem
			{
				iidx0 = 99103;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99103;
				stype0 = 1;
				taid = 1;
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
				taid = 2;
				esctype = 0;
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
				conv = 252109;
				ctype = 1;
				idx = 6111106;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 252101;
				cnt0 = 8;
				ectype = -1;
				etype = 0;
				qtidx = 252120;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 252108;
				m3widx = 14;
				m1fy = "0.000000";
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "-729.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-371.000000";
				m4ttip = 252115;
				area = 252101;
				goal = 252104;
				m0fz = "-1110.000000";
				m0widx = 14;
				m1ttip = 252115;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-1110.000000";
				m3ttip = 252115;
				scitem = 99103;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-711.000000";
				m2fx = "-438.000000";
				m2ttip = 252115;
				m4widx = 14;
				m0fx = "-796.000000";
				m0ttip = 252115;
				m3pat = 2;
				m4pat = 2;
				title = 252102;
				m1fx = "-684.000000";
				m2fz = "-956.000000";
				sort = 252105;
				m4fz = "-537.000000";
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 252107;
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
				npcidx = "6111106;";
			}
			CDboTSCheckClrQst
			{
				and = "2520;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99103;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 252108;
			gtype = 2;
			oklnk = 2;
			area = 252101;
			goal = 252104;
			sort = 252105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 252102;
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 252114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 252101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3181108;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 252107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99103;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
}

