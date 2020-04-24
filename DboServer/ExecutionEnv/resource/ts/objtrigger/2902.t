CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2902;
	title = 290202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 18;
			elnk = 255;
			nextlnk = 254;
			prelnk = "13;";

			CDboTSActCustomEvt
			{
				ceid = 290201;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "18;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 18;
			prelnk = "0;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 290418;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 290207;
			nolnk = 255;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckCustomEvent
			{
				qid = 2902;
			}
			CDboTSClickObject
			{
				objidx = "2155;2156;2157;2158;2159;2160;";
				widx = 1;
			}
		}
	}
}

