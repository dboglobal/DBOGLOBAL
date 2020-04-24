CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 3758;
	title = 375802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 102;
			elnk = 255;
			nextlnk = 254;
			prelnk = "101;";

			CDboTSActItem
			{
				iidx0 = 98035;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 1205;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 375809;
				ctype = 1;
				idx = 8031401;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 98035;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 375808;
				area = 375801;
				goal = 375804;
				m0fz = "-5654.000000";
				m0widx = 15;
				scitem = 98035;
				sort = 375805;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "-3872.000000";
				m0ttip = 375815;
				rwd = 100;
				taid = 1;
				title = 375802;
				gtype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 375807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "4953103;";
			}
			CDboTSCheckClrQst
			{
				and = "3753;";
				flink = 0;
				not = 0;
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
			cid = 254;
			prelnk = "102;";
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
			cont = 375808;
			gtype = 1;
			oklnk = 2;
			area = 375801;
			goal = 375804;
			sort = 375805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 375802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 375814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 375801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4953103;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98035;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 375807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

