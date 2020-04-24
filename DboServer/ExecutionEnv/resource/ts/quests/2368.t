CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2368;
	title = 236802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 4;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2368;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 98017;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98017;
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
			prelnk = "5;4;";
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
			stdiag = 236807;
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
				and = "2367;";
				flink = 1;
				flinknextqid = "2369;";
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
				conv = 236809;
				ctype = 1;
				idx = 6111106;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2368;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActItem
			{
				iidx0 = 98017;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 236808;
				area = 236801;
				goal = 236804;
				m0fz = "-389.809998";
				m0widx = 14;
				scitem = 98017;
				sort = 236805;
				stype = 2;
				m0fy = "-29.559999";
				m0pat = 0;
				grade = 132203;
				m0fx = "743.590027";
				m0ttip = 236815;
				rwd = 100;
				taid = 1;
				title = 236802;
				gtype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 236814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 236801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5731106;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 537;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 236808;
			gtype = 2;
			oklnk = 2;
			area = 236801;
			goal = 236804;
			sort = 236805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 236802;
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
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98017;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
	}
}

