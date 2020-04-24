CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2365;
	title = 236502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 236507;
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
				npcidx = "6111106;";
			}
			CDboTSCheckClrQst
			{
				and = "2364;";
				flink = 1;
				flinknextqid = "2366;";
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 588;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1337;
				taid = 1;
				type = 1;
			}
			CDboTSActItem
			{
				iidx0 = 98016;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 236520;
			}
			CDboTSActItem
			{
				iidx0 = 98016;
				stype0 = 1;
				taid = 3;
				type = 1;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1337;
				taid = 2;
				type = 0;
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
				iidx0 = 98016;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 236509;
				ctype = 1;
				idx = 6111106;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2365;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 236508;
				area = 236501;
				goal = 236504;
				m0fz = "-389.809998";
				m0widx = 14;
				scitem = 98016;
				sort = 236505;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "743.590027";
				m0ttip = 236515;
				rwd = 100;
				taid = 1;
				title = 236502;
				gtype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 236514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 236501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1337;
			}
			CDboTSClickNPC
			{
				npcidx = "6111106;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;0;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 236508;
			gtype = 2;
			oklnk = 2;
			area = 236501;
			goal = 236504;
			sort = 236505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 236502;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
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

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1337;
				taid = 1;
				type = 1;
			}
			CDboTSActItem
			{
				iidx0 = 98016;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

