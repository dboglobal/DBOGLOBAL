CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 945;
	title = 94502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 94507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 33;
			}
			CDboTSClickNPC
			{
				npcidx = "3172202;";
			}
			CDboTSCheckClrQst
			{
				and = "940;";
				flink = 1;
				flinknextqid = "948;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 94514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 94501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172202;";
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
				conv = 94509;
				ctype = 1;
				idx = 3172202;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 6612201;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 94508;
				m1fx = "4080.520020";
				m1fy = "0.000000";
				sort = 94505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-205.550003";
				grade = 132203;
				m0fx = "4199.779785";
				m0ttip = 94515;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 94502;
				gtype = 2;
				area = 94501;
				goal = 94504;
				m0fz = "-374.880005";
				m0widx = 1;
				m1ttip = 94515;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 94508;
			gtype = 2;
			oklnk = 2;
			area = 94501;
			goal = 94504;
			sort = 94505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 94502;
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
				itype = 0;
			}
		}
	}
}

