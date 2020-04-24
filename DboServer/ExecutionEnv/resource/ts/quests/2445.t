CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2445;
	title = 244502;

	CNtlTSGroup
	{
		gid = 0;

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
			stdiag = 244507;
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
				npcidx = "4072301;";
			}
			CDboTSCheckClrQst
			{
				and = "2434;";
				flink = 1;
				flinknextqid = "2446;";
				not = 0;
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
				id = 553;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 244514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 244501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072301;";
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
				conv = 244509;
				ctype = 1;
				idx = 4072301;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2445;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 244508;
				gtype = 2;
				area = 244501;
				goal = 244504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 244505;
				stype = 2;
				taid = 1;
				title = 244502;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2445;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 244508;
			gtype = 2;
			oklnk = 2;
			area = 244501;
			goal = 244504;
			sort = 244505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 244502;
		}
	}
}

