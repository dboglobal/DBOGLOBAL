CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 6;
	rq = 0;
	tid = 1608;
	title = 160802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 160807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "6203101;";
			}
			CDboTSCheckClrQst
			{
				and = "1583;1607;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 1608;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 1608;
				start = 1;
				taid = 1;
				uof = 1;
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
				id = 339;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 6;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 160809;
				ctype = 1;
				idx = 6203101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 160808;
				gtype = 1;
				area = 160801;
				goal = 160804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 160805;
				stype = 1;
				taid = 1;
				title = 160802;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 160814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 160801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172401;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			cont = 160808;
			gtype = 1;
			oklnk = 2;
			area = 160801;
			goal = 160804;
			sort = 160805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 160802;
		}
	}
}

