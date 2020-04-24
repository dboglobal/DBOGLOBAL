CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2300;
	title = 230002;

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
				conv = 230009;
				ctype = 1;
				idx = 3182109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 230008;
				gtype = 1;
				area = 230001;
				goal = 230004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 230005;
				stype = 1;
				taid = 1;
				title = 230002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 230007;
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
				npcidx = "3182109;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2302;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 230008;
			gtype = 1;
			oklnk = 2;
			area = 230001;
			goal = 230004;
			sort = 230005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 230002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 230014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 230001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142303;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

