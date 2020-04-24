CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3755;
	title = 375502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 375509;
				ctype = 1;
				idx = 3184106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 375508;
				gtype = 1;
				area = 375501;
				goal = 375504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 375505;
				stype = 2;
				taid = 1;
				title = 375502;
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
			cont = 375508;
			gtype = 1;
			oklnk = 2;
			area = 375501;
			goal = 375504;
			sort = 375505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 375502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 375514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 375501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3191101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 375507;
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
				npcidx = "3184106;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3756;";
				not = 0;
			}
		}
	}
}

