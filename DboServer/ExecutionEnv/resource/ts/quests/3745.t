CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3745;
	title = 374502;

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
				conv = 374509;
				ctype = 1;
				idx = 3173206;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 374508;
				gtype = 1;
				area = 374501;
				goal = 374504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 374505;
				stype = 2;
				taid = 1;
				title = 374502;
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
			cont = 374508;
			gtype = 1;
			oklnk = 2;
			area = 374501;
			goal = 374504;
			sort = 374505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 374502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 374514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 374501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1161101;";
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
			stdiag = 374507;
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
				npcidx = "3173206;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3746;";
				not = 0;
			}
		}
	}
}

