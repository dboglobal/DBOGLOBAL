CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2145;
	title = 214502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 214509;
				ctype = 1;
				idx = 3242401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 214508;
				gtype = 1;
				area = 214501;
				goal = 214504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 214505;
				stype = 2;
				taid = 1;
				title = 214502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 214507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "3242401;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2146;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 214508;
			gtype = 1;
			oklnk = 2;
			area = 214501;
			goal = 214504;
			sort = 214505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 214502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 214514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 214501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4953101;";
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
	}
}

