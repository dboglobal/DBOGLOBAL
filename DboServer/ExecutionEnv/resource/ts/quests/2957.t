CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2957;
	title = 295702;

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
				conv = 295709;
				ctype = 1;
				idx = 4751112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 295708;
				area = 295701;
				goal = 295704;
				m0fz = "-4307.490234";
				m0widx = 1;
				scitem = -1;
				sort = 295705;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "2668.610107";
				m0ttip = 295715;
				rwd = 100;
				taid = 1;
				title = 295702;
				gtype = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1535;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
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
				itype = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActExcCGroup
			{
				gid = 21;
				rtype = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActExcCGroup
			{
				gid = 255;
				rtype = 0;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
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
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 295708;
			gtype = 3;
			oklnk = 2;
			area = 295701;
			goal = 295704;
			sort = 295705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 295702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 295714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 295701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751112;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 295707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4751112;";
			}
			CDboTSCheckClrQst
			{
				and = "2956;";
				flink = 1;
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 295807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1535;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 21;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActRegCTiming
			{
				0_midx = 15792101;
				rtype = 1;
				etype = 0;
				flags = 1;
				ntidx = 29572;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
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

