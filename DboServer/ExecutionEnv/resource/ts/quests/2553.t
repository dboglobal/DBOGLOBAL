CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2553;
	title = 255302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 572;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 255321;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;6;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98026;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 98026;
				stype0 = 1;
				taid = 1;
				type = 1;
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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 255307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "3189105;";
			}
			CDboTSCheckClrQst
			{
				and = "2552;";
				flink = 1;
				flinknextqid = "2554;";
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
				conv = 255309;
				ctype = 1;
				idx = 3189105;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 98026;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 255308;
				area = 255301;
				goal = 255304;
				m0fz = "-4510.799805";
				m0widx = 1;
				scitem = -1;
				sort = 255305;
				stype = 2;
				m0fy = "-139.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "-2400.360107";
				m0ttip = 255315;
				rwd = 100;
				taid = 1;
				title = 255302;
				gtype = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 255314;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 255301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 255308;
			gtype = 3;
			oklnk = 2;
			area = 255301;
			goal = 255304;
			sort = 255305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 255302;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98026;
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
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
	}
}

