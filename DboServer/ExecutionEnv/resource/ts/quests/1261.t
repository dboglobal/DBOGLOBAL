CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 5;
	rq = 0;
	tid = 1261;
	title = 126102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 98008;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActItem
			{
				iidx0 = 98008;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 286;
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
				conv = 126109;
				ctype = 1;
				idx = 4372101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 126108;
				area = 126101;
				goal = 126104;
				m0fz = "3032.000000";
				m0widx = 1;
				scitem = 98008;
				sort = 126105;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "2264.000000";
				m0ttip = 126115;
				rwd = 100;
				taid = 1;
				title = 126102;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 126108;
			gtype = 2;
			oklnk = 2;
			area = 126101;
			goal = 126104;
			sort = 126105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 126102;
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
			desc = 126114;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 126101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 126107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "4372101;";
			}
			CDboTSCheckClrQst
			{
				and = "1236;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 255;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 255;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98008;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

